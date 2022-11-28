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
#include "Math/Integer.h"

namespace Jam
{
    template <typename T,
              U64 L        = 8 * sizeof(T) * (8 * sizeof(T)),
              U64 D        = L* L,
              U64        M = L - 1>
    struct TPair
    {
        T a{0};
        T b{0};

        constexpr TPair()                = default;
        constexpr TPair(const TPair& ab) = default;

        constexpr explicit TPair(const T& x)
        {
            factor(x, a, b);
        }

        constexpr void factor(const T value, T& x, T& y)
        {
            T n = 0;
            x = 0, y = 0;
            if (value < 2)
            {
                x = value;
                return;
            }

            while ((x + 1) * (y + 1) != value && ++n < D)
            {
                x = (x + 1) % L;
                if (x % L == M)
                    y = (y + 1) % L;
            }
        }

        constexpr T combined() const
        {
            return (a + 1) * (b + 1);
        }

        constexpr TPair<T> factor(const T value) const
        {
            TPair<T> ret = {0, 0};
            factor(value, ret.a, ret.b);
            return ret;
        }
    };

    using UPair32 = TPair<U32>;
    using UPair16 = TPair<U16>;
    using UPair8  = TPair<U8>;

}  // namespace Jam
