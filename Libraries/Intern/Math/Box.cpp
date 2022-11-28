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

#include "Math/Box.h"

#include "Lg.h"

namespace Jam
{
    void Box::merge(const R32 x, const R32 y)
    {
        if (x < x1)
            x1 = x;
        if (x > x2)
            x2 = x;
        if (y < y1)
            y1 = y;
        if (y > y2)
            y2 = y;
    }

    void Box::expand(const R32 x, const R32 y)
    {
        x1 -= x;
        x2 += x;
        y1 -= y;
        y2 += y;
    }

    void Box::clear()
    {
        x1 = y1 = Infinity;
        x2 = y2 = -Infinity;
    }

    void Box::print() const
    {
        Lg::print("{", x1, ",", y1, ",", x2, ",", y2, "}");
    }

    void Box::makeArray(Vec2F* dest) const
    {
        dest[0] = v1();
        dest[1] = v2();
        dest[2] = v3();
        dest[3] = v4();
    }

}  // namespace Jam
