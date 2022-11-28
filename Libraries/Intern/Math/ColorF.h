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
#include "Math/Real.h"

namespace Jam
{
    class Color
    {
    public:
        R32 r{1}, g{0}, b{1}, a{1};

        const R32* ptr() const
        {
            return &r;
        }

        bool equals(const Color& color) const
        {
            const R32 ca = 4 * r + 3 * g + 2 * b + a;
            const R32 cb = 4 * color.r + 3 * color.g + 2 * color.b + color.a;
            return Jam::equals(ca, cb);
        }
    };

    constexpr Color Black = {0, 0, 0, 1};
    constexpr Color White = {1, 1, 1, 1};
    constexpr Color Empty = {1, 0, 1, 1};

}  // namespace Jam
