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
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#endif
#include "Utils/Char.h"
#include <bitset>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <limits>
#include <sstream>

#include "StreamMethods.h"
#include "Utils/String.h"

namespace Jam
{
    size_t Char::length(const char* input)
    {
        return input && *input ? strlen(input) : 0;
    }

    bool Char::equals(const char* a, const char* b, const size_t max)
    {
        if (!a || !b)
            return false;

        if (*a != *b)
            return false;

        return strncmp(a, b, std::min(max, length(b))) == 0;
    }

    bool Char::equals(const char*  a,
                      const size_t al,
                      const char*  b,
                      const size_t bl)
    {
        if (!a || !b)
            return false;
        if (*a != *b)
            return false;
        if (al != bl)
            return false;
        return strncmp(a, b, std::min(al, bl)) == 0;
    }

    int16_t Char::toInt16(const char* in, const int16_t def, const int base)
    {
        if (in && *in)
        {
            const long long v = std::strtoll(in, nullptr, base);

            if (v > std::numeric_limits<int16_t>::max())
                return std::numeric_limits<int16_t>::max();

            if (v < std::numeric_limits<int16_t>::min())
                return std::numeric_limits<int16_t>::min();

            return static_cast<int16_t>(v);
        }
        return def;
    }

    int32_t Char::toInt32(const char* in, const int32_t def, const int base)
    {
        if (in && *in)
        {
            const long long v = std::strtoll(in, nullptr, base);
            if (v > std::numeric_limits<int32_t>::max())
                return std::numeric_limits<int32_t>::max();
            if (v < std::numeric_limits<int32_t>::min())
                return std::numeric_limits<int32_t>::min();
            return static_cast<int32_t>(v);
        }
        return def;
    }

    int64_t Char::toInt64(const char* in, const int64_t def, const int base)
    {
        if (in && *in)
            return std::strtoll(in, nullptr, base);
        return def;
    }

    uint16_t Char::toUint16(const char* in, const uint16_t def, const int base)
    {
        if (in && *in)
        {
            const unsigned long long v = std::strtoull(in, nullptr, base);
            if (v > std::numeric_limits<uint16_t>::max())
                return std::numeric_limits<uint16_t>::max();
            return static_cast<uint16_t>(v);
        }
        return def;
    }

    uint32_t Char::toUint32(const char* in, const uint32_t def, const int base)
    {
        if (in && *in)
        {
            const unsigned long long v = std::strtoull(in, nullptr, base);
            if (v > std::numeric_limits<uint32_t>::max())
                return std::numeric_limits<uint32_t>::max();
            return static_cast<uint32_t>(v);
        }
        return def;
    }

    uint64_t Char::toUint64(const char* in, const uint64_t def, const int base)
    {
        if (in && *in)
            return std::strtoull(in, nullptr, base);
        return def;
    }

    int16_t Char::toInt16(const String& in, const int16_t def, const int base)
    {
        return toInt16(in.c_str(), def, base);
    }

    int32_t Char::toInt32(const String& in, const int32_t def, const int base)
    {
        return toInt32(in.c_str(), def, base);
    }

    int64_t Char::toInt64(const String& in, const int64_t def, const int base)
    {
        return toInt64(in.c_str(), def, base);
    }

    uint16_t Char::toUint16(const String&  in,
                            const uint16_t def,
                            const int      base)
    {
        return toUint16(in.c_str(), def, base);
    }

    uint32_t Char::toUint32(const String&  in,
                            const uint32_t def,
                            const int      base)
    {
        return toUint32(in.c_str(), def, base);
    }

    uint64_t Char::toUint64(const String&  in,
                            const uint64_t def,
                            const int      base)
    {
        return toUint64(in.c_str(), def, base);
    }

    bool Char::isNullOrEmpty(const char* in)
    {
        return !in || !*in;
    }

    bool Char::toBool(const char* in)
    {
        if (in)
        {
            switch (*in)
            {
            case 't':
                return strncmp(in, "true", 4) == 0;
            case 'y':
                return strncmp(in, "yes", 3) == 0;
            case '1':
                return strncmp(in, "1", 1) == 0;
            default:
                break;
            }
        }
        return false;
    }

    float Char::toFloat(const char* in, const float& def)
    {
        if (in && *in)
            return std::strtof(in, nullptr);
        return def;
    }

    double Char::toDouble(const char* in, const double& def)
    {
        if (in && *in)
        {
            InputStringStream ss(in);
            DoublePrint a(0);
            a.from(ss);
            return (double)a;//std::strtod(in, nullptr);
        }
        return def;
    }

    bool Char::toBool(const String& in)
    {
        return toBool(in.c_str());
    }

    float Char::toFloat(const String& in, const float& def)
    {
        return toFloat(in.c_str(), def);
    }

    double Char::toDouble(const String& in, const double& def)
    {
        return toDouble(in.c_str(), def);
    }

    void Char::toString(String& dest, const float v)
    {
        std::stringstream stream;
        stream << v;
        dest = stream.str();
    }

    void Char::toString(String& dest, const double v)
    {
        std::stringstream stream;
        stream << v;
        dest = stream.str();
    }

    void Char::toString(String& dest, const bool v)
    {
        dest.reserve(2);
        dest.resize(0);
        if (v)
            dest.push_back('1');
        else
            dest.push_back('0');
    }

    String Char::toString(const float v)
    {
        String copyOnReturn;
        toString(copyOnReturn, v);
        return copyOnReturn;
    }

    String Char::toString(const double v)
    {
        String copyOnReturn;
        toString(copyOnReturn, v);
        return copyOnReturn;
    }

    String Char::toString(const bool v)
    {
        String copyOnReturn;
        toString(copyOnReturn, v);
        return copyOnReturn;
    }

    void Char::toString(String& dest, const int16_t v)
    {
        std::ostringstream oss;
        oss << v;
        dest = oss.str();
    }

    String Char::toString(const int16_t v)
    {
        String copyOnReturn;
        toString(copyOnReturn, v);
        return copyOnReturn;
    }

    void Char::toString(String& dest, const int32_t v)
    {
        std::ostringstream oss;
        oss << v;
        dest = oss.str();
    }

    String Char::toString(const int32_t v)
    {
        String copyOnReturn;
        toString(copyOnReturn, v);
        return copyOnReturn;
    }

    void Char::toString(String& dest, const int64_t v)
    {
        std::ostringstream oss;
        oss << v;
        dest = oss.str();
    }

    String Char::toString(const int64_t v)
    {
        String copyOnReturn;
        toString(copyOnReturn, v);
        return copyOnReturn;
    }

    void Char::toString(String& dest, const uint16_t v)
    {
        std::ostringstream oss;
        oss << v;
        dest = oss.str();
    }

    String Char::toString(const uint16_t v)
    {
        String copyOnReturn;
        toString(copyOnReturn, v);
        return copyOnReturn;
    }

    void Char::toString(String& dest, const uint32_t v)
    {
        std::ostringstream oss;
        oss << v;
        dest = oss.str();
    }

    String Char::toString(const uint32_t v)
    {
        String copyOnReturn;
        toString(copyOnReturn, v);
        return copyOnReturn;
    }

    void Char::toString(String& dest, const uint64_t v)
    {
        std::ostringstream oss;
        oss << v;
        dest = oss.str();
    }

    String Char::toString(const uint64_t v)
    {
        String copyOnReturn;
        toString(copyOnReturn, v);
        return copyOnReturn;
    }

    void Char::toHexString(String& dest, const uint8_t v)
    {
        std::ostringstream oss;
        oss << std::setfill('0');
        oss << std::setw(2);
        oss << std::hex << (int)v;
        dest = oss.str();
    }

    String Char::toHexString(const uint8_t v)
    {
        String copyOnReturn;
        toHexString(copyOnReturn, v);
        return copyOnReturn;
    }

    void Char::toHexString(String& dest, const uint16_t v)
    {
        std::ostringstream oss;
        oss << std::setfill('0');
        oss << std::setw(4);
        oss << std::hex << v;
        dest = oss.str();
    }

    String Char::toHexString(const uint16_t v)
    {
        String copyOnReturn;
        toHexString(copyOnReturn, v);
        return copyOnReturn;
    }

    void Char::toHexString(String& dest, const uint32_t v)
    {
        std::ostringstream oss;
        oss << std::setfill('0');
        oss << std::setw(8);
        oss << std::hex << v;
        dest = oss.str();
    }

    String Char::toHexString(const uint32_t v)
    {
        String copyOnReturn;
        toHexString(copyOnReturn, v);
        return copyOnReturn;
    }

    void Char::toHexString(String& dest, const uint64_t v)
    {
        std::ostringstream oss;
        oss << std::setfill('0');
        oss << std::setw(16);
        oss << std::hex << v;
        dest = oss.str();
    }

    String Char::toHexString(const uint64_t v)
    {
        String copyOnReturn;
        toHexString(copyOnReturn, v);
        return copyOnReturn;
    }

    void Char::toBinaryString(String& dest, const uint8_t v)
    {
        const std::bitset<8> bs(v);
        dest = bs.to_string();
    }

    String Char::toBinaryString(const uint8_t v)
    {
        String copyOnReturn;
        toBinaryString(copyOnReturn, v);
        return copyOnReturn;
    }

    void Char::toBinaryString(String& dest, const uint16_t v)
    {
        const std::bitset<16> bs(v);
        dest = bs.to_string();
    }

    String Char::toBinaryString(const uint16_t v)
    {
        String copyOnReturn;
        toBinaryString(copyOnReturn, v);
        return copyOnReturn;
    }

    void Char::toBinaryString(String& dest, const uint32_t v)
    {
        const std::bitset<32> bs(v);
        dest = bs.to_string();
    }

    String Char::toBinaryString(const uint32_t v)
    {
        String copyOnReturn;
        toBinaryString(copyOnReturn, v);
        return copyOnReturn;
    }

    void Char::toBinaryString(String& dest, const uint64_t v)
    {
        const std::bitset<64> bs(v);
        dest = bs.to_string();
    }

    String Char::toBinaryString(const uint64_t v)
    {
        String copyOnReturn;
        toBinaryString(copyOnReturn, v);
        return copyOnReturn;
    }

    int Char::hexToDec(const char v)
    {
        switch (v)
        {
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            return 10 + (v - 'A');
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return 10 + (v - 'a');
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return v - '0';
        default:
            return 0;
        }
    }
}  // namespace Jam
