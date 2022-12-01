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
#include "State/FrameStack/GridLayer.h"
#include "State/FrameStack/RenderContext.h"

namespace Jam::Editor::State
{

    GridLayer::GridLayer() :
        BaseLayer(GridType)
    {
        _minor.reserve(125);
        _major.reserve(25);
        _center.reserve(2);
    }

    void GridLayer::render(RenderContext& canvas)
    {
        screenGrid(canvas, _screen.axis());
    }

    void GridLayer::screenGrid(RenderContext& canvas,
                               const Axis&    axis)
    {
        Box bb;
        _screen.corners(bb);

        const Vec2F& origin   = _screen.origin();
        const Vec2F  multiple = _screen.aspectMultiple();

        bb = (bb * multiple) + origin;
        bb = bb + origin;

        const Vec2F offset = _screen.aspectOffset() + origin;

        constexpr I32   major = 5;
        constexpr Vec2I m1    = {major, major + 1};

        const Vec2F maj = {
            (bb.x2 - bb.x1) / (R32(m1.y) - R32(axis.x.mod(m1.x))),
            (bb.y2 - bb.y1) / (R32(m1.y) - R32(axis.y.mod(m1.x))),
        };
        const Vec2F min = {
            maj.x / (R32(m1.y) - R32(axis.x.mod(m1.x))),
            maj.y / (R32(m1.y) - R32(axis.y.mod(m1.x))),
        };

        stepGrid(bb.x1, bb.y1, bb.x2, bb.y2, maj, offset, _major);
        stepGrid(bb.x1, bb.y1, bb.x2, bb.y2, min, offset, _minor);

        canvas.selectColor(_minorColor);
        canvas.drawLines(_minor);
        _minor.resizeFast(0);

        canvas.selectColor(_majorColor);
        canvas.drawLines(_major);
        _major.resizeFast(0);

        axisLine(offset.x + (bb.x2 - bb.x1) * Half, 1, _center);
        axisLine(offset.y + (bb.y2 - bb.y1) * Half, 0, _center);

        canvas.selectColor(_originColor);
        canvas.drawLines(_center);
        _center.resizeFast(0);

        canvas.selectColor(_textColor);
        stepLabels(canvas, bb.x1, bb.y1, bb.x2, bb.y2, maj, offset, axis);
    }

    void GridLayer::stepGrid(
        const R32    x1,
        const R32    y1,
        const R32    x2,
        const R32    y2,
        const Vec2F& ax,
        const Vec2F& offset,
        LineBuffer&  buffer) const
    {
        const R32 hw = offset.x + (x2 - x1) * Half;
        const R32 hh = offset.y + (y2 - y1) * Half;

        R32 s0 = hw;
        while (s0 < _screen.w())
        {
            axisLine(s0, 1, buffer);
            s0 += ax.x;
        }

        s0 = hw;
        while (s0 > 0)
        {
            axisLine(s0, 1, buffer);
            s0 -= ax.x;
        }

        s0 = hh;
        while (s0 < _screen.h())
        {
            axisLine(s0, 0, buffer);
            s0 += ax.y;
        }

        s0 = hh;
        while (s0 > 0)
        {
            axisLine(s0, 0, buffer);
            s0 -= ax.y;
        }
    }

    void GridLayer::stepLabels(
        const RenderContext& canvas,
        const R32            x1,
        const R32            y1,
        const R32            x2,
        const R32            y2,
        const Vec2F&         ax,
        const Vec2F&         offset,
        const Axis&          gx) const
    {
        const R32 hw = offset.x + (x2 - x1) * Half;
        const R32 hh = offset.y + (y2 - y1) * Half;
        const R32 cx = ax.x;
        const R32 cy = ax.y;

        R32 s0 = hw + ax.x;
        R32 v  = cx;
        while (s0 < _screen.w())
        {
            canvas.axisValue(I32(s0), I32(hh), gx.x.pointByI(v), true);
            s0 += ax.x;
            v += ax.x;
        }

        s0 = hw - ax.x;
        v  = -cx;
        while (s0 > 0)
        {
            canvas.axisValue(I32(s0), I32(hh), gx.x.pointByI(v), true);
            s0 -= ax.x;
            v -= ax.x;
        }

        s0 = hh + ax.y;
        v  = cy;
        while (s0 < _screen.h())
        {
            canvas.axisValue(I32(hw), I32(s0), gx.y.pointByI(-v), false);
            s0 += ax.y;
            v += ax.y;
        }

        s0 = hh - ax.y;
        v  = -cy;
        while (s0 > 0)
        {
            canvas.axisValue(I32(hw), I32(s0), gx.y.pointByI(-v), false);
            s0 -= ax.y;
            v -= ax.y;
        }
    }

    void GridLayer::axisLine(
        const R32&  step,
        const int   dir,
        LineBuffer& dest) const
    {
        if (dest.size() > 256)
            return;

        if (dir)
            dest.push_back(QLineF{
                QPointF{(qreal)step,                0.0},
                QPointF{(qreal)step, (qreal)_screen.h()},
            });
        else
            dest.push_back(QLineF{
                QPointF{               0.0, (qreal)step},
                QPointF{(qreal)_screen.w(), (qreal)step},
            });
    }

}  // namespace Jam::Editor::State