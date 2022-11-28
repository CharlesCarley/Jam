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
#include "ColorF.h"
#include "Math/Integer.h"
#include "Math/Real.h"

namespace Jam
{
#if JAM_ENDIAN == JAM_ENDIAN_BIG
    constexpr U8 Rb = 0;
    constexpr U8 Gb = 1;
    constexpr U8 Bb = 2;
    constexpr U8 Ab = 3;
#else
    constexpr U8 Rb = 3;
    constexpr U8 Gb = 2;
    constexpr U8 Bb = 1;
    constexpr U8 Ab = 0;
#endif

    class IColor
    {
    private:
        UFrag32 fragment{0};

    public:
        IColor() = default;

        explicit IColor(const Color& c)
        {
            fragment.b[Ab] = U8(c.a * 255);
            fragment.b[Rb] = U8(c.r * 255);
            fragment.b[Gb] = U8(c.g * 255);
            fragment.b[Bb] = U8(c.b * 255);
        }

        explicit IColor(const U32& c)
        {
            UFrag32 o;
            o.whole = c;

            fragment.b[Ab] = o.b[Ab];
            fragment.b[Rb] = o.b[Rb];
            fragment.b[Gb] = o.b[Gb];
            fragment.b[Bb] = o.b[Bb];
        }

        IColor(const U8& r, const U8& g, const U8& b, const U8& a = 0xFF)
        {
            fragment.b[Ab] = a;
            fragment.b[Rb] = r;
            fragment.b[Gb] = g;
            fragment.b[Bb] = b;
        }

        U32 whole() const
        {
            return fragment.whole;
        }

        void setAsRgb(const U8 col)
        {
            fragment.b[Ab] = 0xFF;
            fragment.b[Rb] = col;
            fragment.b[Gb] = col;
            fragment.b[Bb] = col;
        }

        U8 r() const
        {
            return fragment.b[Rb];
        }

        U8 g() const
        {
            return fragment.b[Gb];
        }

        U8 b() const
        {
            return fragment.b[Bb];
        }

        U8 a() const
        {
            return fragment.b[Ab];
        }
    };
}  // namespace Jam
