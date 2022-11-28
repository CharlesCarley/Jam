#ifdef _WIN32
    #include <Windows.h>
#endif

#include <iostream>
#include <sstream>
#include "Utils/Console.h"
#include "Utils/Definitions.h"

namespace Jam
{
    static ConsoleColor gForeground = CS_WHITE;
    static ConsoleColor gBackground = CS_BLACK;

#ifndef _WIN32
    static uint8_t MapColor(const ConsoleColor& col, bool bg)
    {
        uint8_t color = 0;
        switch (col)
        {
        case CS_BLACK:
            color = 30;
            break;
        case CS_DARK_BLUE:
            color = 34;
            break;
        case CS_DARK_GREEN:
            color = 32;
            break;
        case CS_DARK_CYAN:
            color = 36;
            break;
        case CS_DARK_RED:
            color = 31;
            break;
        case CS_DARK_MAGENTA:
            color = 35;
            break;
        case CS_DARK_YELLOW:
            color = 33;
            break;
        case CS_LIGHT_GREY:
            color = 90;
            break;
        case CS_GREY:
            color = 37;
            break;
        case CS_BLUE:
            color = 94;
            break;
        case CS_GREEN:
            color = 92;
            break;
        case CS_CYAN:
            color = 96;
            break;
        case CS_RED:
            color = 91;
            break;
        case CS_MAGENTA:
            color = 95;
            break;
        case CS_YELLOW:
            color = 93;
            break;
        case CS_WHITE:
            color = 0;
            break;
        }
        if (bg)
            color += 10;
        return color;
    }

#endif

    static void SetColor()
    {
#ifdef _WIN32
        SetConsoleTextAttribute(
            GetStdHandle(STD_OUTPUT_HANDLE),
            (UCHAR)((UCHAR)gBackground * 16 + (UCHAR)gForeground));
#else
        uint8_t color[2] = {
            MapColor(gForeground, false),
            MapColor(gBackground, true),
        };
        if (gBackground != CS_BLACK)
            printf("\033[%dm\033[%dm", color[0], color[1]);
        else
            printf("\033[%dm", color[0]);
#endif
    }

    void Console::read()
    {
        std::cin.get();
    }

    void Console::read(int8_t& v)
    {
        std::cin >> v;
    }

    void Console::read(uint8_t& v)
    {
        std::cin >> v;
    }

    void Console::read(int16_t& v)
    {
        std::cin >> v;
    }

    void Console::read(uint16_t& v)
    {
        std::cin >> v;
    }

    void Console::read(int32_t& v)
    {
        std::cin >> v;
    }

    void Console::read(uint32_t& v)
    {
        std::cin >> v;
    }

    void Console::read(int64_t& v)
    {
        std::cin >> v;
    }

    void Console::read(uint64_t& v)
    {
        std::cin >> v;
    }

    void Console::read(String& v)
    {
        std::cin >> v;
    }

    void Console::readLine(String& v)
    {
        std::getline(std::cin, v);
    }

    void Console::setForeground(const ConsoleColor col)
    {
        gForeground = col;
    }

    void Console::setBackground(const ConsoleColor col)
    {
        gBackground = col;
    }

    void Console::resetColor()
    {
#ifndef _WIN32
        gForeground = CS_WHITE;
        gBackground = CS_BLACK;
        printf("\033[0m");
#else
        gForeground = CS_WHITE;
        gBackground = CS_BLACK;
        SetColor();
#endif
    }

    void Console::write(const String& str)
    {
        SetColor();
        std::cout << str;

#if defined(_WIN32) && defined(_DEBUG)
        if (IsDebuggerPresent())
            OutputDebugString(str.c_str());
#endif
    }

    void Console::writeLine(const String& str)
    {
        SetColor();
        std::cout << str << std::endl;
        resetColor();
#if defined(_WIN32) && defined(_DEBUG)
        if (IsDebuggerPresent())
            OutputDebugString((str + "\n").c_str());
#endif
    }

    void Console::writeError(const String& str)
    {
        setForeground(CS_RED);
        SetColor();
        std::cerr << str << std::endl;
        resetColor();
    }

    void printHexLine(OStream&     dest,
                      const char*  buffer,
                      const size_t offs,
                      const size_t max)
    {
        if (!buffer)
            return;

        size_t        i, j;
        unsigned char ch;

        for (i = 0x00; i < 0x10; ++i)
        {
            j = i + offs;
            if (i % 0x09 == 0x08)
                dest << ' ';
            if (j >= max)
                dest << "00 ";
            else
            {
                ch = buffer[j];
                dest << std::setfill('0')
                     << std::setw(2)
                     << std::uppercase
                     << std::hex << (unsigned int)ch
                     << ' ';
            }
        }

        dest << '|';
        for (i = 0x00; i < 0x10; ++i)
        {
            j = i + offs;
            if (j >= max)
                dest << '.';
            else
            {
                ch = buffer[j];
                if (ch >= 0x20 && ch < 0x7F)
                    dest << (char)ch;
                else
                    dest << '.';
            }
        }
        dest << '|';
        dest << '\n';
    }

    void printHex(OStream& dest, const char* buffer, const uint32_t len)
    {
        if (buffer)
        {
            for (size_t i = 0; i < len; i += 0x10)
                printHexLine(dest, buffer, i, len);
        }
    }

    void printHex(const char* buffer, const uint32_t len)
    {
        printHex(std::cout, buffer, len);
    }

    void Console::hexdump(const String& str)
    {
        printHex(str.c_str(), (uint32_t)str.size());
    }

    void Console::hexdump(const OutputStringStream& str)
    {
        hexdump(str.str());
    }

    void Console::hexdump(
        OStream&       dest,
        const void*    p,
        const uint32_t len)
    {
        if (p)
            printHex(dest, (const char*)p, len);
    }

    void Console::hexdump(const void* p, const uint32_t len)
    {
        if (p)
            hexdump(std::cout, p, len);
    }
    
    void printBinLine(OStream&     dest,
                      const char*  buffer,
                      const size_t offs,
                      const size_t max)
    {
        if (!buffer)
            return;

        size_t        i, j;
        unsigned char ch;

        for (i = 0x00; i < 0x8; ++i)
        {
            j = i + offs;
            if (i % 0x05 == 0x04)
                dest << ' ';
            if (j >= max)
                dest << "0000 ";
            else
            {
                ch = buffer[j];
                std::bitset<4> set(ch);
                dest << set.to_string()
                     << ' ';
            }
        }

        dest << '|';
        for (i = 0x00; i < 0x8; ++i)
        {
            j = i + offs;
            if (j >= max)
                dest << '.';
            else
            {
                ch = buffer[j];
                if (ch >= 0x20 && ch < 0x7F)
                    dest << (char)ch;
                else
                    dest << '.';
            }
        }
        dest << '|';
        dest << '\n';
    }

    void printBin(OStream& dest, const char* buffer, const uint32_t len)
    {
        if (buffer)
        {
            for (size_t i = 0; i < len; i += 0x8)
                printBinLine(dest, buffer, i, len);
        }
    }

    void Console::bindump(const String& str)
    {
        bindump(str.c_str(), (uint32_t)str.size());
    }
    
    void Console::bindump(const OutputStringStream& str)
    {
        bindump(str.str());
    }

    void Console::bindump(
        OStream&       dest,
        const void*    p,
        const uint32_t len)
    {
        if (p)
            printBin(dest, (const char*)p, len);
    }

    void Console::bindump(const void* p, const uint32_t len)
    {
        if (p)
            bindump(std::cout, p, len);
    }
    void Console::writeLine(const int64_t& i)
    {
        OutputStringStream oss;
        oss << i;
        writeLine(oss);
    }

    void Console::writeLine(const double& i)
    {
        OutputStringStream oss;
        oss << i;
        writeLine(oss);
    }

    void Console::writeLine(const OutputStringStream& str)
    {
        writeLine(str.str());
    }

    void Console::writeError(const OutputStringStream& str)
    {
        writeError(str.str());
    }

    void Console::nl()
    {
        writeLine("");
    }

    // This meant to be switched off later
#define USE_EXCEPTION_BREAK
    void Console::debugBreak()
    {
#ifdef USE_EXCEPTION_BREAK
    #if defined(_WIN32) && defined(_DEBUG)

        if (IsDebuggerPresent())
            DebugBreak();
    #endif
#endif  // USE_EXCEPTION_BREAK
    }
}  // namespace Jam
