#include <cstdint>
#include <iomanip>
#include "Utils/CommandLine/Parser.h"
#include "Utils/Console.h"
#include "Utils/Exception.h"
#include "Utils/FileSystem.h"
#include "Utils/TextStreamWriter.h"
#include "Utils/Win32/CrtUtils.h"
using namespace std;

namespace Jam::ResourceCompiler
{
    enum Option
    {
        OptOutputFileName,
        OptNameSpace,
        OptionsMax,
    };

    constexpr CommandLine::Switch Switches[OptionsMax] = {
        {
            OptOutputFileName,
            'o',
            nullptr,
            "Specify the output file name",
            true,
            1,
        },
        {
            OptNameSpace,
            'n',
            nullptr,
            "Specify a root namespace",
            true,
            1,
        },
    };

    struct Resource
    {
        String data;
        size_t size{};
    };

    using ResourceMap = std::unordered_map<String, Resource>;

    class Application
    {
    private:
        String      _namespace;
        String      _output;
        StringArray _input;
        ResourceMap _resources;

    public:
        Application() = default;

        bool parse(const int argc, char** argv)
        {
            CommandLine::Parser p;
            if (p.parse(argc, argv, Switches, OptionsMax) < 0)
                return false;

            _output    = p.string(OptOutputFileName, 0, "Resources");
            _namespace = p.string(OptNameSpace, 0, "");
            _input     = p.arguments();

            if (_input.empty())
                throw MessageException("No input files");
            return true;
        }

        void loadInput(const String& input)
        {
            InputFileStream in(input, std::ios_base::binary);
            if (!in.is_open())
                throw InputException("Failed to open the supplied file '", input, '\'');

            String name = FilePath(input).stem().string();

            name = StringUtils::toLowerFirst(name);

            OutputStringStream srcImpl;
            OutputStringStream bufferImpl;

            size_t len = 0;

            bufferImpl << std::setfill(' ') << std::setw(0x0B) << ' ';

            char ch;
            while (in.read(&ch, 1))
            {
                const int v = ch;
                bufferImpl << "0x"
                           << std::setfill('0')
                           << std::setw(2)
                           << std::hex << (uint32_t)(uint8_t)v
                           << ",";

                if (len % 13 == 12)
                {
                    bufferImpl << std::endl;
                    bufferImpl << std::setfill(' ') << std::setw(0x0B) << ' ';
                }

                ++len;
            }

            if (len > 0)
            {
                srcImpl << std::setw(0x07) << ' ' << "dest = {" << std::endl;
                srcImpl << bufferImpl.str() << std::endl;
                srcImpl << std::setw(0x07) << ' ' << "};" << std::endl;
            }

            _resources[name] = {srcImpl.str(), len};
        }

        void writeHeader(OStream& out)
        {
            WriteUtils::write(out, 0x00, "#pragma once");
            WriteUtils::write(out, 0x00, "#include <vector>");

            String namespaceName;

            if (!_namespace.empty())
                Su::join(namespaceName, _namespace);
            else
                namespaceName = "Resources";

            WriteUtils::write(out, 0x00, "namespace ", namespaceName);
            WriteUtils::write(out, 0x00, '{');
            WriteUtils::write(out, 0x04, "using ByteArray = std::vector<uint8_t>;");
            WriteUtils::write(out, 0x00, '\n');
            WriteUtils::write(out, 0x04, "class Resource");
            WriteUtils::write(out, 0x04, '{');
            WriteUtils::write(out, 0x04, "public:");

            bool first = true;

            for (auto& [name, source] : _resources)
            {
                if (!first)
                    WriteUtils::newLine(out, 1);
                first = false;
                WriteUtils::write(out,
                                  0x08,
                                  "static void get",
                                  StringUtils::toUpperFirst(name),
                                  "(ByteArray &dest);");
            }

            WriteUtils::write(out, 0x04, "};");
            WriteUtils::write(out, 0x00, "} // namespace ", namespaceName);
        }

        void writeSource(OStream& out)
        {
            WriteUtils::writeLine(out, 0x00, 2, "#include \"", _output, ".h\"");
            String namespaceName;

            if (!_namespace.empty())
                Su::join(namespaceName, _namespace);
            else
                namespaceName = "Resources";

            WriteUtils::write(out, 0x00, "namespace ", namespaceName);
            WriteUtils::write(out, 0x00, '{');

            bool first = true;

            for (auto& [name, source] : _resources)
            {
                if (!first)
                    WriteUtils::newLine(out, 1);
                first = false;

                String methodName = Su::join("get", StringUtils::toUpperFirst(name));
                WriteUtils::write(out,
                                  0x04,
                                  "void Resource::",
                                  methodName,
                                  "(ByteArray &dest)");
                WriteUtils::write(out, 0x04, '{');
                WriteUtils::write(out, 0x00, source.data);
                if (source.size <= 0)
                {
                    WriteUtils::write(out, 0x08, "// TODO: No valid data was in the file; its length computed to zero ");
                    WriteUtils::write(out, 0x08, "dest.clear();");
                }
                WriteUtils::write(out, 0x04, '}');
            }
            WriteUtils::write(out, 0x00, "} // namespace ", namespaceName);
        }

        int go()
        {
            for (const String& input : _input)
                loadInput(input);

            OutputFileStream src(Su::join("", _output, ".h"));
            if (src.is_open())
                writeHeader(src);

            src.close();
            src.open(Su::join("", _output, ".cpp"));

            if (src.is_open())
                writeSource(src);

            return 0;
        }
    };

}  // namespace Jam::ResourceCompiler

int main(const int argc, char** argv)
{
    Jam::CrtTestMemory();
    int rc = 0;
    try
    {
        Jam::ResourceCompiler::Application app;
        if (app.parse(argc, argv))
            rc = app.go();
    }
    catch (std::exception& ex)
    {
        Jam::Console::writeError(ex.what());
    }

    Jam::CrtDump();
    return rc;
}
