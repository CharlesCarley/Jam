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
#include "Math/Vec2F.h"

namespace Jam
{
    class RectF
    {
    public:
        R32 x{0}, y{0}, w{1}, h{1};

        RectF()               = default;
        RectF(const RectF& v) = default;

        Vec2F lt() const { return {x, y}; }
        Vec2F rb() const { return {x + w, y + h}; }

        Vec2F extent() const { return {w, h}; }

        Vec2F halfExtent() const { return {w * Half, h * Half}; }

        R32 l() const { return x; }
        R32 t() const { return x; }
        R32 r() const { return x + w; }
        R32 b() const { return x + h; }

        RectF operator*(const Vec2F& sz) const
        {
            const R32 x1 = x * sz.x;
            const R32 y1 = y * sz.y;
            const R32 x2 = (x + w) * sz.x;
            const R32 y2 = (y + h) * sz.y;
            return {x1, y1, x2 - x1, y2 - y1};
        }

        void corners(R32& x1, R32& y1, R32& x2, R32& y2) const
        {
            x1 = l();
            y1 = t();
            x2 = r();
            y2 = b();
        }

        void print() const;
    };

}  // namespace Jam
