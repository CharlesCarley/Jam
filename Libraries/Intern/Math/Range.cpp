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
#include "Math/Range.h"

namespace Jam
{
    void Range::clear()
    {
        min = Infinity;
        max = -Infinity;
    }

    void Range::zero()
    {
        min = 0;
        max = 0;
    }

    void Range::compare(const R32 v)
    {
        if (v < min)
            min = v;
        if (v > max)
            max = v;
    }

    Range Range::intersection(const Range& r) const
    {
        // https://www.desmos.com/calculator/tnyxvcvfc8

        const R32 rmi = fabs(min - r.min);
        const R32 rma = fabs(max - r.max);
        return {minF(min, r.min) + rmi, maxF(max, r.max) - rma};
    }

    void Range::print() const
    {
        Console::writeLine("[", min, ",", max, "]");
    }
}  // namespace Jam
