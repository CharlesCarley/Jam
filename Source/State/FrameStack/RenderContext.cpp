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
#include "RenderContext.h"
#include "Interface/Areas/OutputArea.h"
#include "Interface/Style/Palette.h"
#include "Math/Axis.h"
#include "Math/Box.h"
#include "Math/Lg.h"
#include "Utils/StreamMethods.h"

namespace Jam
{
    // temp
    using namespace Editor::Log;

    RenderContext::RenderContext(QPainter* painter, Screen screen) :
        _screen{std::move(screen)},
        _painter{painter}
    {
        _size = toVec2I(_screen.viewport().extent());
        _painter->setRenderHint(QPainter::Antialiasing, true);
        _painter->setRenderHint(QPainter::TextAntialiasing, true);
    }

    RenderContext::~RenderContext() = default;

    void RenderContext::clear(const U8 red,
                              const U8 green,
                              const U8 blue,
                              const U8 alpha) const
    {
        if (isNotValid())
            return;

        _painter->fillRect(QRect{0, 0, _size.x, _size.y},
                           QColor(red, green, blue, alpha));
    }

    void RenderContext::flush() const
    {
        if (isNotValid())
            return;
    }

    void RenderContext::selectColor(const U32& col, int w)
    {
        if (isNotValid())
            return;

        _color = IColor{col};
        _pen.setWidth(w);
        _pen.setColor(QColor{
            _color.r(),
            _color.g(),
            _color.b(),
            _color.a(),
        });
        _painter->setPen(_pen);
    }

    void RenderContext::selectColor(const Color& col, int w)
    {
        if (isNotValid())
            return;

        _color = IColor{col};
        _pen.setWidth(w);
        _pen.setColor(QColor{
            _color.r(),
            _color.g(),
            _color.b(),
            _color.a(),
        });
        _painter->setPen(_pen);
    }

    void RenderContext::selectColor(const QColor& col, int w)
    {
        _pen.setWidth(w);
        _pen.setColor(col);
        _painter->setPen(_pen);
    }

    void RenderContext::drawLine(int x1, int y1, int x2, int y2) const
    {
        if (isNotValid())
            return;

        drawLine(R32(x1), R32(y1), R32(x2), R32(y2));
    }

    void RenderContext::drawLine(R32 x1, R32 y1, R32 x2, R32 y2) const
    {
        if (isNotValid())
            return;

        const Vec2F& o = {0.f, 0};
        _painter->drawLine(
            QPointF{qreal(x1 + o.x), qreal(y1 + o.y)},
            QPointF{qreal(x2 + o.x), qreal(y2 + o.y)});
    }

    void RenderContext::drawPoint(int x0, int y0) const
    {
        if (isNotValid())
            return;
    }

    void RenderContext::drawPoint(int x0, int y0, int scale) const
    {
        if (isNotValid())
            return;
    }

    void RenderContext::copyBuffer(void* src, U32 w, U32 h) const
    {
        if (isNotValid())
            return;
    }

    void RenderContext::drawVec2F(const int    x0,
                                  const int    y0,
                                  const Vec2F& v,
                                  U8           p) const
    {
        OutputStringStream oss;
        oss << SetS({"x", "y"})
            << Equ()
            << SetF({v.x, v.y}, (p * 2) + 2, p, true);
        _painter->drawText(x0, y0, QString(oss.str().c_str()));
    }

    void RenderContext::axisValue(
        const int  x0,
        const int  y0,
        const R32& v,
        const bool hor) const
    {
        OutputStringStream oss;
        oss << FloatDPrint(v, 8, 4);
        const QString text = QString::fromStdString(oss.str());

        QRect r = _painter->fontMetrics()
                      .tightBoundingRect(text)
                      .translated(x0, y0);

        if (hor)
            r = r.translated(-r.width() >> 1, r.height() << 1);
        else
            r = r.translated(0, r.height());

        _painter->drawText(r.x(), r.y(), text);
    }

    void RenderContext::drawAxisF(
        const int   x0,
        const int   y0,
        const Axis& v) const
    {
        OutputStringStream oss;
        oss << SetS({"xn", "xd", "xn", "yn"})
            << Equ()
            << SetU({v.x.n(), v.x.d()})
            << SetU({v.y.n(), v.y.d()});
        _painter->drawText(x0, y0, QString(oss.str().c_str()));
    }

    void RenderContext::axisLine(const R32& step, const int dir, LineBuffer& dest) const
    {
        if (isNotValid() || dest.size() > 256)
            return;

        if (dir)
            dest.push_back(QLineF{
                QPointF{(qreal)step,            0.0},
                QPointF{(qreal)step, (qreal)_size.y},
            });
        else
            dest.push_back(QLineF{
                QPointF{           0.0, (qreal)step},
                QPointF{(qreal)_size.x, (qreal)step},
            });
    }

    void RenderContext::screenGrid(const Axis& axis,
                                   const U32&  majorColor,
                                   const U32&  minorColor,
                                   const U32&  centerColor,
                                   const U32&  textColor)
    {
        if (isNotValid())
            return;

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

        selectColor(minorColor);
        _painter->drawLines(_minor.data(), _minor.sizeI());
        _minor.resizeFast(0);

        selectColor(majorColor);
        _painter->drawLines(_major.data(), _major.sizeI());
        _major.resizeFast(0);

        axisLine(offset.x + (bb.x2 - bb.x1) * Half, 1, _center);
        axisLine(offset.y + (bb.y2 - bb.y1) * Half, 0, _center);

        selectColor(centerColor);
        _painter->drawLines(_center.data(), _center.sizeI());
        _center.resizeFast(0);

        selectColor(textColor);
        stepLabels(bb.x1, bb.y1, bb.x2, bb.y2, maj, offset, axis);
    }

    void RenderContext::stepGrid(
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
        while (s0 < _size.rx())
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
        while (s0 < _size.ry())
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

    void RenderContext::stepLabels(
        const R32    x1,
        const R32    y1,
        const R32    x2,
        const R32    y2,
        const Vec2F& ax,
        const Vec2F& offset,
        const Axis&  gx) const
    {
        const R32 hw = offset.x + (x2 - x1) * Half;
        const R32 hh = offset.y + (y2 - y1) * Half;
        const R32 cx = ax.x;
        const R32 cy = ax.y;

        R32 s0 = hw + ax.x;
        R32 v  = cx;
        while (s0 < _size.rx())
        {
            axisValue(I32(s0), I32(hh), gx.x.pointByI(v), true);
            s0 += ax.x;
            v += ax.x;
        }

        s0 = hw - ax.x;
        v  = -cx;
        while (s0 > 0)
        {
            axisValue(I32(s0), I32(hh), gx.x.pointByI(v), true);
            s0 -= ax.x;
            v -= ax.x;
        }

        s0 = hh + ax.y;
        v  = cy;
        while (s0 < _size.ry())
        {
            axisValue(I32(hw), I32(s0), gx.y.pointByI(-v), false);
            s0 += ax.y;
            v += ax.y;
        }

        s0 = hh - ax.y;
        v  = -cy;
        while (s0 > 0)
        {
            axisValue(I32(hw), I32(s0), gx.y.pointByI(-v), false);
            s0 -= ax.y;
            v -= ax.y;
        }
    }
}  // namespace Jam
