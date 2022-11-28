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
#include "Utils/ParserBase/ScannerBase.h"
#include "Utils/Char.h"
#include "Utils/ParserBase/ParseError.h"

namespace Jam
{
    ScannerBase::ScannerBase() :
        _stream(nullptr),
        _line(0)
    {
    }

    void ScannerBase::attach(IStream* stream, const PathUtil& file)
    {
        _stream = stream;
        _file   = file;
        _line   = 1;
    }

    void ScannerBase::cleanup()
    {
        _intTable.clear();
        _stringTable.clear();
        _stream = nullptr;
    }

    const String& ScannerBase::string(const size_t& i) const
    {
        return _stringTable.at(i);
    }

    void ScannerBase::string(String& dest, const size_t& i) const
    {
        _stringTable.at(dest, i);
    }

    int ScannerBase::integer(const size_t& i) const
    {
        return _intTable.at(i);
    }

    bool ScannerBase::containsString(const size_t id) const
    {
        return _stringTable.contains(id);
    }

    [[noreturn]] void ScannerBase::syntaxErrorThrow(const String& message) const
    {
        throw ParseError(0, _file.fullPath(), _line, message);
    }

    void ScannerBase::scanLineComment()
    {
        int ch = _stream->peek();
        while (ch != '\r' && ch != '\n')
        {
            ch = _stream->get();
            if (ch == '\r' && _stream->peek() == '\n')
                ch = _stream->get();
        }
        ++_line;
    }

    void ScannerBase::scanMultiLineComment()
    {
        int ch = _stream->peek();

        while (ch > 0)
        {
            ch = _stream->get();
            if (ch == MultiLineCommentStop0 && _stream->peek() == MultiLineCommentStop1)
            {
                _stream->get();
                break;
            }
            if (ch == '\r' || ch == '\n')
            {
                if (ch == '\r' && _stream->peek() == '\n')
                    ch = _stream->get();
                ++_line;
            }
        }
    }

    void ScannerBase::scanAny(String& dest, char seqStart, char seqEnd)
    {
        int ch = _stream->get();
        while (ch != seqStart)
        {
            ch = _stream->get();
            if (ch <= 0)
                syntaxError("end of file scan while searching for ", seqStart);
        }
        ch = _stream->get();

        OutputStringStream oss;
        while (ch != seqEnd)
        {
            if (ch != seqEnd)
            {
                if (ch != '\r' && ch != '\n')
                {
                    if (ch == '\t')
                        ch = ' ';
                    oss << (char)ch;
                }
            }

            ch = _stream->get();
            if (ch <= 0)
                syntaxError("end of file scan while searching for ", seqEnd);

            if (ch == '\r' || ch == '\n')
            {
                if (ch == '\r' && _stream->peek() == '\n')
                    _stream->get();

                ch = _stream->get();
                oss << '\n';
                ++_line;
            }
        }

        dest = oss.str();
    }

    void ScannerBase::extractCode(String& dest, char seqStart, char seqEnd)
    {
        String block;
        scanAny(block, seqStart, seqEnd);

        OutputStringStream oss;

        StringArray lines;
        StringUtils::splitLine(lines, block);
        for (const String& line : lines)
        {
            if (!line.empty())
            {
                String temp;
                StringUtils::trimWs(temp, line);
                if (!temp.empty())
                    oss << temp << '\n';
            }
        }

        const String code = oss.str();

        dest = code.substr(0, code.size() - 1);
    }

    void ScannerBase::scanWhiteSpace() const
    {
        int ch = 0;
        while (ch != -1)
        {
            if (isWhiteSpace(_stream->peek()))
                ch = _stream->get();
            else
                break;
        }
    }
}  // namespace Jam
