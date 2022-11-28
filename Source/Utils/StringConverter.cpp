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
#include "StringConverter.h"
#include "Utils/Char.h"

namespace Jam
{
    void StringConverter::toVec2F(const String& str, Vec2F& dest)
    {
        R32Array ra;
        ra.reserve(2);
        toR32Array(str, ra);

        if (ra.size() >= 2)
        {
            dest.x = ra[0];
            dest.y = ra[1];
        }
        else
            Console::writeError("Failed to convert input string: ", str);
    }

    R32 RealCh(const char ch)
    {
        return (R32)Char::hexToDec(ch);
    }

    void StringConverter::toColor(const String& txt, Color& dest)
    {
        if (txt.find('#') != String::npos)
        {
            if (txt.at(0) != '#')
                return;

            if (txt.size() == 4)  // R-G-B A=1
            {
                dest.r = 17 * RealCh(txt[1]) / 255.f;
                dest.g = 17 * RealCh(txt[2]) / 255.f;
                dest.b = 17 * RealCh(txt[3]) / 255.f;
                dest.a = 1;
            }
            else if (txt.size() == 7)  // RR-GG-BB-A=1
            {
                dest.r = (16 * RealCh(txt[1]) + RealCh(txt[2])) / 255.f;
                dest.g = (16 * RealCh(txt[3]) + RealCh(txt[4])) / 255.f;
                dest.b = (16 * RealCh(txt[5]) + RealCh(txt[6])) / 255.f;
                dest.a = 1;
            }
            else if (txt.size() == 9)  // RR-GG-BB-AA
            {
                dest.r = (16 * RealCh(txt[1]) + RealCh(txt[2])) / 255.f;
                dest.g = (16 * RealCh(txt[3]) + RealCh(txt[4])) / 255.f;
                dest.b = (16 * RealCh(txt[5]) + RealCh(txt[6])) / 255.f;
                dest.a = (16 * RealCh(txt[7]) + RealCh(txt[8])) / 255.f;
            }
        }
        else
        {
            R32Array ra;
            ra.reserve(4);
            toR32Array(txt, ra);

            if (ra.size() >= 4)
            {
                dest.r = ra[0];
                dest.g = ra[1];
                dest.b = ra[2];
                dest.a = ra[3];
            }
            else
                Console::writeError("Failed to convert input string: ", txt);
        }
    }

    void StringConverter::toRectF(const String& str, RectF& dest)
    {
        R32Array ra;
        ra.reserve(4);
        toR32Array(str, ra);

        if (ra.size() >= 4)
        {
            dest.x = ra[0];
            dest.y = ra[1];
            dest.w = ra[2];
            dest.h = ra[3];
        }
        else
            Console::writeError("Failed to convert input string: ", str);
    }

    void StringConverter::toBox(const String& str, Box& dest)
    {
        R32Array ra;
        ra.reserve(4);

        toR32Array(str, ra);

        if (ra.size() >= 4)
        {
            dest.x1 = ra[0];
            dest.y1 = ra[1];
            dest.x2 = ra[2];
            dest.y2 = ra[3];
        }
        else
            Console::writeError("Failed to convert input string: ", str);
    }

    void StringConverter::toR32Array(
        const String& str,
        R32Array&     dest,
        const I8      sep)
    {
        StringArray sa;
        StringUtils::splitRejectEmpty(sa, str, sep);

        dest.reserve(Clamp<U32>(U32(sa.size()), 0, MaxSplit));

        for (const auto& v : sa)
            dest.push_back(Char::toFloat(v));
    }

    void StringConverter::toI32Array(
        const String& str,
        I32Array&     dest,
        const I8      sep)
    {
        StringArray sa;
        StringUtils::splitRejectEmpty(sa, str, sep);

        dest.reserve(Clamp<U32>(U32(sa.size()), 0, MaxSplit));

        for (const auto& v : sa)
            dest.push_back(Char::toInt32(v));
    }

}  // namespace Jam
