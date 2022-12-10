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
        _major.reserve(30);
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
        const Vec2F so = _screen.offset();
        const Vec2F ao = _screen.aspectOffset();

        Box   sb;
        Vec2F ds;
        _screen.unitRect(sb, ds);

        const Vec2F of = ao + so + ds;

        const R32 xd = _screen.axis().xMajor();
        const R32 yd = _screen.axis().yMajor();

        R32 sr = sb.w() * reciprocal(xd, 0);
        if (sr > 0)
        {
            const R32 ix = bb.x1 + fmod(of.x, sr);

            R32 sv = ix;
            while (sv < bb.x2)
            {
                axisLine(sv, XAxis, _major);
                sv += sr;
            }
        }

        sr     = sb.h() * reciprocal(yd, 0);
        if (sr > 0)
        {
            const R32 iy = bb.y1 + fmod(of.y, sr);

            R32 sv = iy;
            while (sv < bb.y2)
            {
                axisLine(sv, YAxis, _major);
                sv += sr;
            }
        }

        canvas.selectColor(_majorColor);
        canvas.drawLines(_major);
        //_major.resizeFast(0);

        canvas.selectColor(_originColor);
        axisLine(sb.cx(), XAxis, _center);
        axisLine(sb.cy(), YAxis, _center);
        canvas.drawLines(_center);
        _center.resizeFast(0);

        // canvas.drawBoxF(sb, 2);
        canvas.selectColor(_textColor);
        // canvas.drawR32(5, 40, "sq", sb.w() / _screen.axis().xMajor());
        // canvas.drawR32(5, 60, "ds", ds.x);
        // canvas.drawR32(5, 80, "r10", axis.x.r10());

        for (const QLineF& pt : _major)
        {
            if (isZero(R32(pt.x2()) - R32(pt.x1())))
            {
                const R32 px = R32(pt.x2());
                const R32 ox = -bb.cx() + px - so.x;
                if (const R32 av = axis.x.pointByI(ox);
                    !isZero(av))
                    canvas.axisValue(px, bb.cy() + so.y, av, true);
            }
            else
            {
                const R32 py = R32(pt.y2());
                const R32 oy = -bb.cy() + py - so.y;
                if (const R32 av = axis.y.pointByI(-oy);
                    !isZero(av))
                {
                    canvas.axisValue(bb.cx() + so.x, py, av, false);
                }
            }
        }
        _major.resizeFast(0);
    }

    void GridLayer::axisLine(
        const R32&  step,
        const int   dir,
        LineBuffer& dest) const
    {
        if (dest.size() > MaxBuf)
            return;

        if (dir == XAxis)
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