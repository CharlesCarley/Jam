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
#include "Math/Screen.h"

namespace Jam
{

    void Screen::zoom(const R32 s, const bool negate)
    {
        Box b;
        corners(b);
        _limit.step(I32(s));
        _limit.print(12);
        b = b + Vec2F{
                    _limit.fraction(),
                    _limit.fraction(),
                };
        setViewport(b.x1, b.y1, b.w(), b.h());
    }

    void Screen::zoomLarge(const R32 s, const bool negate)
    {
        sA += (s * z) * (negate ? R32(-1) : R32(1));

        scale = vp.extent() + sA;

        z = scale.x / vp.w;
        if (isZero(z)) z = Epsilon;
    }

    void Screen::reset()
    {
        sA    = 1;
        o     = _init;
        scale = vp.extent();
        c     = scale * Half;

        if (vp.w > 0)
            z = scale.x / vp.w;
        else
            z = 1;
    }

    void Screen::setViewport(const R32& x, const R32& y, const R32& w, const R32& h)
    {
        vp.x = Max(x, -ScreenMax);
        vp.y = Max(y, -ScreenMax);
        vp.w = Min(w, ScreenMax);
        vp.h = Min(h, ScreenMax);
    }

    void Screen::setViewport(const I32& x, const I32& y, const I32& w, const I32& h)
    {
        setViewport(
            R32(x), R32(y), R32(w), R32(h));
    }
}  // namespace Jam
