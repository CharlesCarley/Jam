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
    void Screen::reset()
    {
        _o = _i;
        _c = _v.halfExtent();
    }

    //Vec2I Screen::scroll() const
    //{
    //    Box   box;
    //    Vec2F offs;
    //    unitRect(box, offs);
    //
    //    return {
    //        I32(box.w() / _a.xMajor()),
    //        I32(box.h() / _a.yMajor()),
    //    };
    //}

    void Screen::setViewport(const RectF& v)
    {
        setViewport(v.x, v.y, v.w, v.h);
    }

    void Screen::setViewport(const I32& x, const I32& y, const I32& w, const I32& h)
    {
        setViewport(R32(x), R32(y), R32(w), R32(h));
    }

    void Screen::unitRect(Box& dest, Vec2F& offs) const
    {
        corners(dest);

        const Vec2F m = aspectMultiple();
        const Vec2F o = aspectOffset() + origin();

        // square it
        dest = {
            dest.x1 * m.x + o.x,
            dest.y1 * m.y + o.y,
            dest.x2 * m.x + o.x,
            dest.y2 * m.y + o.y};

        const R32 smx = _a.xMajor();
        const R32 vx  = smx * makeDivBy(dest.w() / smx, smx);
        if (isZero(smx))  // should not happen, possible values [4,8,10]
            throw Exception("divide by zero");

        const R32 smy = _a.yMajor();
        const R32 vy  = smy * makeDivBy(dest.h() / smy, smy);
        if (isZero(smy))  // should not happen, possible values [4,8,10]
            throw Exception("divide by zero");

        offs.x = (dest.w() - vx) / 2;
        offs.y = (dest.h() - vy) / 2;

        dest.expand(-offs.x, -offs.y);
    }

    void Screen::makeSquare()
    {
        Box dest;
        corners(dest);
        const Vec2F m = aspectMultiple();
        const Vec2F o = aspectOffset() + origin();

        const R32 dx1 = dest.x1 * m.x + o.x;
        const R32 dx2 = dest.x2 * m.x + o.x;
        const R32 vs  = makeDivBy((dx2 - dx1) / 10, 10);
        _a.set(0, I32(vs));
        _a.set(1, I32(vs));
    }

    void Screen::setViewport(const R32& x, const R32& y, const R32& w, const R32& h)
    {
        _v.x = Max(x, -ScreenMax);
        _v.y = Max(y, -ScreenMax);
        _v.w = Min(w, ScreenMax);
        _v.h = Min(h, ScreenMax);
    }

}  // namespace Jam
