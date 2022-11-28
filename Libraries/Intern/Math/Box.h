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
#include "Math/Real.h"
#include "Math/RectF.h"

namespace Jam
{
    class Box
    {
    public:
        R32 x1{Infinity},
            y1{Infinity},
            x2{-Infinity},
            y2{-Infinity};

        Box() = default;
        Box(const R32 nx1,
            const R32 ny1,
            const R32 nx2,
            const R32 ny2) :
            x1{nx1}, y1{ny1}, x2{nx2}, y2{ny2}
        {
        }

        explicit Box(const RectF& r) :
            x1{r.x}, y1{r.y}, x2{r.x + r.w}, y2{r.y + r.h}

        {
        }

        void merge(R32 x, R32 y);

        void expand(R32 x, R32 y);

        void clear();

        void print() const;

        R32 w() const;

        R32 h() const;

        Vec2F v1() const;

        Vec2F v2() const;

        Vec2F v3() const;

        Vec2F v4() const;

        void makeArray(Vec2F* dest) const;

        Box operator*(const Vec2F& sz) const;

        Vec2F center() const;

        Vec2F halfSize() const;

        Box operator+(const Vec2F& o) const
        {
            return {x1 + o.x, y1 + o.y, x2 + o.x, y2 + o.y};
        }
    };

    inline R32 Box::w() const
    {
        return fabs(x2 - x1);
    }

    inline R32 Box::h() const
    {
        return fabs(y2 - y1);
    }

    inline Vec2F Box::v1() const
    {
        return {x1, y1};
    }

    inline Vec2F Box::v2() const
    {
        return {x2, y1};
    }

    inline Vec2F Box::v3() const
    {
        return {x2, y2};
    }

    inline Vec2F Box::v4() const
    {
        return {x1, y2};
    }

    inline Box Box::operator*(const Vec2F& sz) const
    {
        return {x1 * sz.x, y1 * sz.y, x2 * sz.x, y2 * sz.y};
    }

    inline Vec2F Box::center() const
    {
        return {x1 + Half * w(), y1 + Half * h()};
    }

    inline Vec2F Box::halfSize() const
    {
        return {Half * w(), Half * h()};
    }

}  // namespace Jam
