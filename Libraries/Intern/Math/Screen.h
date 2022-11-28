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
        Vec2F o{0.f, 0};        // (0,0) of the coordinate system
        R32   z{1.f};           // computed viewport change
        R32   sA{1.f};          // the current amount to scale the viewport by
        Vec2F scale{1.f, 1};    // storage to compute the change in m_viewport.extent()
        Vec2F c{0.f, 0};        // offset from viewport center
        RectF vp{0, 0, 1, 1};   // fixed bounds of the viewport ([x] [x] [L] [L])
        RectF asp{0, 0, 1, 1};  // fixed bounds of the viewport ([x] [x] [L] [L])
        Vec2F _init{0.f, 0};    // the 'home' origin
        Slice _limit{};         // the range of the scale function

    public:
        Screen() = default;

        void zoom(const R32 s, const bool negate);

        void zoomLarge(const R32 s, const bool negate);

        void pan(const R32 px, const R32 py)
        {
            o.x += px * z;
            o.y += py * z;
        }

        void setOrigin(const R32 px, const R32 py)
        {
            o.x = px;
            o.y = py;
        }

        void corners(Box& box) const
        {
            box = {left(), top(), right(), bottom()};
        }


        void reset();

        const R32& getZoom() const
        {
            return z;
        }

        const R32& getScale() const
        {
            return sA;
        }

        Vec2F aspectMultiple() const
        {
            // Defined as the small over the large
            // in order to trim off the largest side 
            // of the rectangle to form a square.
            if (vp.w > vp.h)
                return {vp.h / vp.w, 1.f};
            return {1.f, vp.w / vp.h};
        }

        Vec2F aspectOffset() const
        {
            // Trims off the remainder then returns half
            // of it to recenter it in the larger rectangle.
            const Vec2F mul = aspectMultiple();
            if (vp.w > vp.h)
                return {(vp.w - vp.w * mul.x) * Half, 0.f};
            return {0.f, (vp.h - vp.h * mul.y) * Half};
        }

        R32 xOffs() const
        {
            return (-c.x + o.x + scale.x * Half) / z;
        }

        R32 yOffs() const
        {
            return (-c.y + o.y + scale.y * Half) / z;
        }

        void xToView(R32& x) const
        {
            x = x / z + xOffs();
        }

        R32 getViewX(const R32& x) const
        {
            R32 xCpy = x;
            xToView(xCpy);
            return xCpy;
        }

        void yToView(R32& y) const
        {
            y = y * reciprocal(z + yOffs(), 1);
        }

        R32 getViewY(const R32& y) const
        {
            R32 yCpy = y;
            yToView(yCpy);
            return yCpy;
        }

        void xToScreen(R32& x) const
        {
            x = (x - xOffs()) * z;
        }

        R32 getScreenX(const R32& x) const
        {
            R32 xCpy = x;
            xToScreen(xCpy);
            return xCpy;
        }

        void yToScreen(R32& y) const
        {
            y = (y - yOffs()) * z;
        }

        R32 getScreenY(const R32& y) const
        {
            R32 xCpy = y;
            yToScreen(xCpy);
            return xCpy;
        }

        void pointToScreen(Vec2F& pt) const
        {
            xToScreen(pt.x);
            yToScreen(pt.y);
        }

        void pointToView(Vec2F& pt) const
        {
            xToView(pt.x);
            yToView(pt.y);
        }

        Vec2F offset() const
        {
            return {xOffs(), yOffs()};
        }

        const Vec2F& origin() const
        {
            return o;
        }

        const Vec2F& extent() const
        {
            return scale;
        }

        const Vec2F& getCenter() const
        {
            return c;
        }

        const RectF& viewport() const
        {
            return vp;
        }

        const R32& left() const
        {
            return vp.x;
        }

        const R32& top() const
        {
            return vp.y;
        }

        R32 right() const
        {
            return vp.r();
        }

        R32 bottom() const
        {
            return vp.b();
        }

        const R32& width() const
        {
            return vp.w;
        }

        const R32& viewportHeight() const
        {
            return vp.h;
        }

        void setScaleLimit(const R32& lMin, const R32& lMax)
        {
            _limit.n(I32(lMax));
            _limit.d(I32(lMin));
        }

        void init(const Vec2F& o)
        {
            _init = o;
        }

        void setViewport(const RectF& v)
        {
            vp = v;
        }

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
