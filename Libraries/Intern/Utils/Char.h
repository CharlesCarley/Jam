/*
-------------------------------------------------------------------------------
    Copyright (c) Charles Carley.

  This software is provided 'as-is', without any
express or implied warranty. In no event will the
authors be held liable for any damages arising from the
use of this software.

  Permission is granted to anyone to use this software
for any purpose, including commercial applications, and
to alter it and redistribute it freely, subject to the
following restrictions:

  1. The origin of this software must not be
misrepresented; you must not claim that you wrote the
original software. If you use this software in a
product, an acknowledgment in the product documentation
would be appreciated but is not required.
  2. Altered source versions must be plainly marked as
such, and must not be misrepresented as being the
original software.
  3. This notice may not be removed or altered from any
source distribution.
-------------------------------------------------------------------------------
*/
#pragma once
#include <cstdint>
#include "Utils/String.h"

namespace Jam
{
    class Char
    {
    public:
        static size_t length(const char* input);

        static bool equals(const char* a, const char* b, size_t max);
        static bool equals(const char* a, size_t al, const char* b, size_t bl);

        static int16_t toInt16(const char* in,
                               int16_t     def  = -1,
                               int         base = 10);

        static int32_t toInt32(const char* in,
                               int32_t     def  = -1,
                               int         base = 10);

        static int64_t toInt64(const char* in,
                               int64_t     def  = -1,
                               int         base = 10);

        static uint16_t toUint16(const char* in,
                                 uint16_t    def  = (uint16_t)-1,
                                 int         base = 10);

        static uint32_t toUint32(const char* in,
                                 uint32_t    def  = (uint32_t)-1,
                                 int         base = 10);

        static uint64_t toUint64(const char* in,
                                 uint64_t    def  = (uint64_t)-1,
                                 int         base = 10);

        static int16_t toInt16(const String& in,
                               int16_t       def  = -1,
                               int           base = 10);

        static int32_t toInt32(const String& in,
                               int32_t       def  = -1,
                               int           base = 10);

        static int64_t toInt64(const String& in,
                               int64_t       def  = -1,
                               int           base = 10);

        static uint16_t toUint16(const String& in,
                                 uint16_t      def  = (uint16_t)-1,
                                 int           base = 10);

        static uint32_t toUint32(const String& in,
                                 uint32_t      def  = (uint32_t)-1,
                                 int           base = 10);

        static uint64_t toUint64(const String& in,
                                 uint64_t      def  = (uint64_t)-1,
                                 int           base = 10);

        static bool isNullOrEmpty(const char* in);

        static bool toBool(const char* in);

        static float toFloat(const char* in, const float& def = 0.f);

        static double toDouble(const char* in, const double& def = 0.0);

        static bool toBool(const String& in);

        static float toFloat(const String& in, const float& def = 0.f);

        static double toDouble(const String& in, const double& def = 0.0);

        static void toString(String& dest, float v);

        static void toString(String& dest, double v);

        static void toString(String& dest, bool v);

        static void toString(String& dest, int16_t v);

        static void toString(String& dest, int32_t v);

        static void toString(String& dest, int64_t v);

        static void toString(String& dest, uint16_t v);

        static void toString(String& dest, uint32_t v);

        static void toString(String& dest, uint64_t v);

        static String toString(float v);

        static String toString(double v);

        static String toString(bool v);

        static String toString(int16_t v);

        static String toString(int32_t v);

        static String toString(int64_t v);

        static String toString(uint16_t v);

        static String toString(uint32_t v);

        static String toString(uint64_t v);

        static void toHexString(String& dest, uint8_t v);

        static void toHexString(String& dest, uint16_t v);

        static void toHexString(String& dest, uint32_t v);

        static void toHexString(String& dest, uint64_t v);

        static String toHexString(uint8_t v);

        static String toHexString(uint16_t v);

        static String toHexString(uint32_t v);

        static String toHexString(uint64_t v);

        static void toBinaryString(String& dest, uint8_t v);

        static void toBinaryString(String& dest, uint16_t v);

        static void toBinaryString(String& dest, uint32_t v);

        static void toBinaryString(String& dest, uint64_t v);

        static String toBinaryString(uint8_t v);

        static String toBinaryString(uint16_t v);

        static String toBinaryString(uint32_t v);

        static String toBinaryString(uint64_t v);

        static int hexToDec(char v);
    };

    inline bool isNewLine(const int constant)
    {
        return constant == '\r' || constant == '\n';
    }

    inline bool isWhiteSpace(const int ch)
    {
        return ch == ' ' || ch == '\t' || isNewLine(ch);
    }

    inline bool isWs(const int ch)
    {
        return ch == ' ' || ch == '\t' || isNewLine(ch);
    }

    inline bool isDecimal(const int ch)
    {
        return ch >= '0' && ch <= '9';
    }

    inline bool isLetter(const int ch)
    {
        return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
    }

    using Ch = Char;

}  // namespace Jam
