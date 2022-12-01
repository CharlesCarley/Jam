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

#include "Axis.h"
#include "RectF.h"
#include "Slice.h"
#include "Vec2.h"

namespace Jam
{
    constexpr R32 ScreenMax = 2048;
    constexpr R32 ScreenMin = 64;

    class Screen
    {
    private:
        Vec2F _o{0.f, 0};      // (0,0) of the coordinate system
        Vec2F _c{0.f, 0};      // offset to viewport center
        RectF _v{0, 0, 1, 1};  // fixed bounds of the viewport ([x] [x] [L] [L])
        Vec2F _i{0.f, 0};      // the 'home' origin
        Axis  _axis{};

    public:
        Screen() = default;

        void translate(const R32 px, const R32 py)
        {
            _o.x += px;
            _o.y += py;
        }

        void setOrigin(const R32 px, const R32 py)
        {
            _o.x = px;
            _o.y = py;
        }

        void corners(Box& box) const
        {
            box = {left(), top(), right(), bottom()};
        }

        void reset();

        Vec2F aspectMultiple() const
        {
            // Defined as the small over the large
            // in order to trim off the largest side
            // of the rectangle to form a square.
            if (_v.w > _v.h)
                return {_v.h / _v.w, 1.f};
            return {1.f, _v.w / _v.h};
        }

        Vec2F aspectOffset() const
        {
            // Trims off the remainder then returns half
            // of it to recenter it in the larger rectangle.
            const Vec2F mul = aspectMultiple();
            if (_v.w > _v.h)
                return {(_v.w - _v.w * mul.x) * Half, 0.f};
            return {0.f, (_v.h - _v.h * mul.y) * Half};
        }

        Vec2F offset() const
        {
            return {
                (-_c.x + _o.x + _v.w * Half),
                (-_c.y + _o.y + _v.h * Half),
            };
        }

        const Vec2F& origin() const
        {
            return _o;
        }

        [[deprecated("use size instead")]] Vec2F extent() const
        {
            return _v.extent();
        }

        Vec2F size() const
        {
            return _v.extent();
        }

        void setXStep(const I32 x)
        {
            _axis.x.stepI(x);
        }

        void setYStep(const I32 y)
        {
            _axis.y.stepI(y);
        }

        void setXAxis(const R32 v)
        {
            _axis.set(0, {v, 1.f});
        }

        void setYAxis(const R32 v)
        {
            _axis.set(1, {v, 1.f});
        }

        const Vec2F& center() const
        {
            return _c;
        }

        const RectF& viewport() const
        {
            return _v;
        }

        const R32& left() const
        {
            return _v.x;
        }

        const R32& top() const
        {
            return _v.y;
        }

        R32 right() const
        {
            return _v.r();
        }

        R32 bottom() const
        {
            return _v.b();
        }

        const R32& width() const
        {
            return _v.w;
        }

        const R32& viewportHeight() const
        {
            return _v.h;
        }

        void init(const Vec2F& ori)
        {
            _i = ori;
        }
        const Axis& axis() const { return _axis; }

        void setViewport(const RectF& v);

        void setViewport(const R32& x,
                         const R32& y,
                         const R32& w,
                         const R32& h);

        void setViewport(const I32& x,
                         const I32& y,
                         const I32& w,
                         const I32& h);
    };
}  // namespace Jam
