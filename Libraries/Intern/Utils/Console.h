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
#pragma once
#include <bitset>
#include <cstdint>
#include <iomanip>
#include "Utils/String.h"

namespace Jam
{
    enum ConsoleColor
    {
        CS_BLACK = 0,
        CS_DARK_BLUE,
        CS_DARK_GREEN,
        CS_DARK_CYAN,
        CS_DARK_RED,
        CS_DARK_MAGENTA,
        CS_DARK_YELLOW,
        CS_LIGHT_GREY,
        CS_GREY,
        CS_BLUE,
        CS_GREEN,
        CS_CYAN,
        CS_RED,
        CS_MAGENTA,
        CS_YELLOW,
        CS_WHITE,
    };

    class Console
    {
    public:
        static void read();

        static void read(int8_t& v);
        static void read(uint8_t& v);
        static void read(int16_t& v);
        static void read(uint16_t& v);
        static void read(int32_t& v);
        static void read(uint32_t& v);
        static void read(int64_t& v);
        static void read(uint64_t& v);
        static void read(String& v);

        static void readLine(String& v);

        static void setForeground(ConsoleColor col);
        static void setBackground(ConsoleColor col);

        static void resetColor();

        static void write(const String& str);
        static void writeLine(const String& str);
        static void writeLine(const int64_t& i);
        static void writeLine(const double& i);
        static void writeError(const String& str);

        static void hexdump(const String& str);
        static void hexdump(const OutputStringStream& str);
        static void hexdump(OStream& dest, const void* p, uint32_t len);
        static void hexdump(const void* p, uint32_t len);

        static void bindump(const String& str);
        static void bindump(const OutputStringStream& str);
        static void bindump(OStream& dest, const void* p, uint32_t len);
        static void bindump(const void* p, uint32_t len);

        template <typename... Args>
        static void writeLine(const String& str, Args&&... args)
        {
            OutputStringStream oss;
            oss << str;
            ((oss << std::forward<Args>(args)), ...);
            writeLine(oss.str());
        }

        template <typename... Args>
        static void writeError(const String& str, Args&&... args)
        {
            OutputStringStream oss;
            oss << str;
            ((oss << std::forward<Args>(args)), ...);
            writeError(oss.str());
        }

        template <typename... Args>
        static void write(const String& str, Args&&... args)
        {
            OutputStringStream oss;
            oss << str;
            ((oss << std::forward<Args>(args)), ...);
            write(oss.str());
        }

        static void writeLine(const OutputStringStream& str);
        static void writeError(const OutputStringStream& str);

        static void debugBreak();
        static void nl();

        template <typename... Args>
        static void println(Args&&... args)
        {
            OutputStringStream oss;
            ((oss << std::forward<Args>(args)), ...);
            writeLine(oss.str());
        }

        template <typename... Args>
        static void print(Args&&... args)
        {
            OutputStringStream oss;
            ((oss << std::forward<Args>(args)), ...);
            write(oss.str());
        }
    };

    using Out = Console;
    using Con = Console;
    using Dbg = Console;

}  // namespace Jam
