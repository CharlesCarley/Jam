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
#include <cstdint>
#include "Utils/Array.h"

namespace Jam
{
    using I64 = int64_t;
    using I32 = int32_t;
    using I16 = int16_t;
    using I8  = int8_t;

    using U64 = uint64_t;
    using U32 = uint32_t;
    using U16 = uint16_t;
    using U8  = uint8_t;

    constexpr U8 Ax = 0;
    constexpr U8 Ay = 1;
    constexpr U8 Az = 2;
    constexpr U8 Aw = 3;

    union IFrag16
    {
        I16 whole;
        I8  fragment[2];
    };

    union IFrag32
    {
        I32     whole;
        IFrag16 fragment[2];
    };

    union IFrag64
    {
        I64     whole;
        IFrag32 fragment[2];
    };

    union UFrag16
    {
        U16 whole{};
        U8  b[2];
    };

    union UFrag32
    {
        U32 whole{};
        U16 s[2];
        U8  b[4];
    };

    union UFrag64
    {
        U64 whole{};
        U32 i[2];
        U16 s[4];
        U8  b[8];
    };

    template <typename T>
    T Mod(T a, T b)
    {
        return b > 0 ? a % b : 0;
    }

    template <typename T>
    constexpr T square(const T& x)
    {
        return x * x;
    }

    using I32Array = SimpleArray<I32>;

}  // namespace Jam
