/*
-------------------------------------------------------------------------------
    Copyright (c) Charles Carley.

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#include "Utils/CommandLine/Parser.h"
#include <iomanip>
#include <iostream>
#include "Utils/Console.h"
#include "Utils/Definitions.h"
#include "Utils/FileSystem.h"
#include "Utils/Path.h"
#include "Utils/TextStreamWriter.h"

namespace Jam::CommandLine
{
    using namespace std;

    Parser::~Parser()
    {
        for (const ParseOption* op : _options)
            delete op;
    }

    bool Parser::hasSwitch(const String& sw) const
    {
        return _switches.find(sw) != _switches.end();
    }

    bool Parser::setupParse(int argc, char** argv)
    {
        JAM_ASSERT(argv)
        JAM_ASSERT(argc >= 1)

        if (!_programName.empty())  // using as a check for multiple calls
            return false;

        _programName = PathUtil(argv[0]);
        _usedOptions = 0;

        _scanner.clear();
        for (int i = 1; i < argc; ++i)
            _scanner.append(argv[i]);
        return true;
    }

    int Parser::parseImpl()
    {
        String tmpBuffer;
        Token  token;

        tmpBuffer.reserve(16);
        const int rc = parseOptions(token, tmpBuffer);

        if (_usedOptions != _requiredOptions)
        {
            OutputStringStream os;
            os << "missing required options" << endl;
            return writeError(os);
        }
        return rc;
    }

    int Parser::parse(const int      argc,
                      char**         argv,
                      const Switch*  switches,
                      const uint32_t count)
    {
        if (!setupParse(argc, argv))
            return -1;

        if (!initializeSwitches(switches, count))
            return -1;

        return parseImpl();
    }

    int Parser::parseNoSwitch(int argc, char** argv)
    {
        if (!setupParse(argc, argv))
            return -1;
        return parseImpl();
    }

    int Parser::parseOptions(Token& token, String& tmpBuffer)
    {
        while (token.getType() != TOK_EOS)
        {
            _scanner.lex(token);
            const int type = token.getType();

            if (token.getType() == TOK_ERROR)
            {
                cerr << token.getValue() << endl;
                usage();
                return -1;
            }

            if (type == TOK_SWITCH_SHORT || type == TOK_SWITCH_LONG)
            {
                // grab the next token
                _scanner.lex(token);

                if (token.getType() == TOK_NEXT || token.getType() == TOK_EOS)
                {
                    OutputStringStream os;
                    os << "expected a switch value to follow '-'" << endl;
                    return writeError(os);
                }

                if (token.getValue() == "help" || token.getValue() == "h")
                {
                    usage();
                    return -1;
                }

                auto it = _switches.find(token.getValue());
                if (it == _switches.end())
                {
                    OutputStringStream os;
                    os << "unknown option " << token.getValue() << endl;
                    return writeError(os);
                }

                tmpBuffer.assign(token.getValue());

                ParseOption* opt = it->second;
                opt->makePresent();

                if (!opt->isOptional())
                    _usedOptions++;

                const size_t nArgs = opt->getArgCount();
                if (nArgs > 0)
                {
                    size_t i;
                    for (i = 0; i < nArgs; ++i)
                    {
                        _scanner.lex(token);

                        if (token.getType() != TOK_IDENTIFIER)
                        {
                            OutputStringStream os;
                            os << "invalid argument for option " << tmpBuffer
                               << endl;
                            return writeError(os);
                        }

                        if (token.getValue().empty())
                        {
                            OutputStringStream os;
                            os << "missing argument for option " << tmpBuffer
                               << endl;
                            return writeError(os);
                        }

                        opt->setValue(i, token.getValue());
                    }

                    if (i != nArgs)
                    {
                        OutputStringStream os;
                        os << "not all arguments converted when parsing switch "
                           << tmpBuffer << endl;
                        return writeError(os);
                    }
                }
            }
            else if (type >= TOK_OPTION && type < TOK_EOS)
                _argumentList.push_back(token.getValue());
            else
            {
                if (type != TOK_EOS)
                {
                    OutputStringStream os;
                    os << "unknown option " << token.getValue().c_str() << endl;
                    return writeError(os);
                }
            }
        }
        return 0;
    }

    void Parser::logInput() const
    {
        OutputStringStream oss;
        WriteUtils::writeLine(oss, 0, 1, "Launching: ", programPath());
        WriteUtils::writeLine(oss, 0, 1, "Working Directory: ", currentDirectory());
        WriteUtils::writeLine(oss, 0, 1, "Command line: [", _scanner.getValue(), "]");
        Console::write(oss.str());
    }

    String Parser::programName() const
    {
        return _programName.fileName();
    }

    String Parser::programDirectory() const
    {
        return _programName.fullPath();
    }

    String Parser::currentDirectory()
    {
        return PathUtil(FileSystem::currentPath()).fullPath();
    }

    bool Parser::isPresent(const uint32_t& enumId) const
    {
        if (enumId < _options.size())
            return _options[enumId]->isPresent();
        return false;
    }

    ParseOption* Parser::option(const uint32_t& enumId) const
    {
        if (enumId < _options.size())
            return _options[enumId];
        return nullptr;
    }

    int32_t Parser::int32(const uint32_t& enumId,
                          const size_t    idx,
                          const int32_t   defaultValue,
                          const int32_t   base) const
    {
        if (enumId < _options.size())
        {
            if (_options[enumId] != nullptr)
                return _options[enumId]->getInt(idx, defaultValue, base);
        }
        return defaultValue;
    }

    int64_t Parser::int64(const uint32_t& enumId,
                          const size_t    idx,
                          const int64_t   defaultValue,
                          const int32_t   base) const
    {
        if (enumId < _options.size())
        {
            if (_options[enumId] != nullptr)
                return _options[enumId]->getInt64(idx, defaultValue, base);
        }
        return defaultValue;
    }

    const String& Parser::string(const uint32_t& enumId,
                                 const size_t    idx,
                                 const String&   defaultValue) const
    {
        if (enumId < _options.size())
        {
            if (_options[enumId] != nullptr)
            {
                const String& val = _options[enumId]->getValue(idx);
                if (val.empty())
                    return defaultValue;
                return val;
            }
        }
        return defaultValue;
    }

    void Parser::usage() const
    {
        String usageString;
        usage(usageString);
        Console::writeLine(usageString);
    }

    void Parser::usage(String& dest) const
    {
        const streamsize w = max(_maxLongSwitch + 2, 4);

        OutputStringStream stream;

        stream << "Usage: " << programName() << " <options> <arg[0] .. arg[n]>" << endl
               << endl;
        stream << setw(4) << "-h, --help" << setw(w - 4) << ' '
               << "Display this help message" << endl;

        for (const ParseOption* opt : _options)
        {
            const Switch& switchVal = opt->getSwitch();
            stream << std::setw(4) << ' ';

            if (switchVal.shortSwitch != 0)
            {
                stream << '-' << switchVal.shortSwitch;
                if (switchVal.longSwitch != nullptr)
                    stream << ", ";
                else
                    stream << setw(4) << ' ';
            }
            else
                stream << setw(4) << ' ';

            streamsize space = _maxLongSwitch;
            if (switchVal.longSwitch != nullptr)
            {
                space -= (int)Char::length(switchVal.longSwitch);
                stream << "--" << switchVal.longSwitch;
            }

            stream << setw(space + 2) << ' ';

            if (switchVal.description != nullptr)
            {
                StringArray arr;
                StringUtils::split(arr, switchVal.description, '\n');

                for (size_t i = 0; i < arr.size(); ++i)
                {
                    const String& str = arr[i];
                    stream << str;
                    if (i + 1 < arr.size())
                    {
                        stream << endl
                               << setw(w + 10) << ' ';
                    }
                }
            }
            stream << endl;
        }
        stream << endl;
        dest = stream.str();
    }

    bool Parser::initializeOption(ParseOption* opt, const Switch& sw)
    {
        if (sw.shortSwitch == 0 && sw.longSwitch == nullptr)
        {
            Console::writeError(
                "A switch must have at least a long or short name");
            return false;
        }

        if (sw.shortSwitch != 0)
        {
            if (hasSwitch(String(sw.shortSwitch, 1)))
            {
                OutputStringStream stream;
                stream << "Duplicate switch " << sw.shortSwitch;
                Console::writeError(stream);
                return false;
            }
        }

        if (sw.longSwitch != nullptr)
        {
            const String lsw = String(sw.longSwitch);

            _maxLongSwitch = max(_maxLongSwitch, (int)lsw.size());

            if (hasSwitch(lsw))
            {
                OutputStringStream stream;
                stream << "Duplicate switch " << sw.longSwitch;
                Console::writeError(stream);
                return false;
            }
        }

        if (sw.shortSwitch != 0)
            _switches.insert(make_pair(String(1, sw.shortSwitch), opt));

        if (sw.longSwitch != nullptr)
            _switches.insert(make_pair(sw.longSwitch, opt));

        return true;
    }

    bool Parser::initializeSwitches(const Switch* switches, uint32_t count)
    {
        if (switches == nullptr || count == 0 || count == (uint32_t)-1)
            return true;

        count = min<uint32_t>(count, 0x100);

        _options.resize(count);
        bool result = true;

        for (uint32_t i = 0; i < count && result; ++i)
        {
            if (switches[i].id != i)
            {
                Console::writeError("Misaligned switch id");
                result = false;
            }
            else
            {
                if (!switches[i].optional)
                    _requiredOptions++;

                _options[i] = new ParseOption(switches[i]);

                result = initializeOption(_options[i], switches[i]);
            }
        }
        return result;
    }

    int Parser::writeError(const OutputStringStream& stream) const
    {
        Console::writeError(stream);
        usage();
        return -1;
    }

}  // namespace Jam::CommandLine
