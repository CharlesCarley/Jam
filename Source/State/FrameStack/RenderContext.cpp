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
#include "Utils/Char.h"
#include "Utils/StreamMethods.h"

namespace Jam::Editor::State
{

    RenderContext::RenderContext(QPainter* painter,
                                 Screen    screen) :
        _screen{std::move(screen)},
        _painter{painter}
    {
        _text.reserve(32);

        // change this so that it does not have to copy every
        // render call
        Palette::getAccentPalette(_palette);

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

    void RenderContext::selectColor(
        const U32& col,
        const U8   width)
    {
        if (isNotValid())
            return;

        _color = IColor{col};
        _pen.setWidth(width);

        _pen.setColor(QColor{
            _color.r(),
            _color.g(),
            _color.b(),
            _color.a(),
        });
        _painter->setPen(_pen);
    }

    void RenderContext::selectColor(const Color& col, const U8 width)
    {
        if (isNotValid())
            return;

        _color = IColor{col};
        _pen.setWidth(width);
        _pen.setColor(QColor{
            _color.r(),
            _color.g(),
            _color.b(),
            _color.a(),
        });
        _painter->setPen(_pen);
    }

    void RenderContext::selectColor(const QColor& col, const U8 width)
    {
        _pen.setWidth(width);
        _pen.setColor(col);
        _painter->setPen(_pen);
    }

    
    void RenderContext::selectColor(const QPalette::ColorRole& col, U8 width)
    {
        selectColor(_palette.color(col), width);
    }


    void RenderContext::drawLine(
        const int x1,
        const int y1,
        const int x2,
        const int y2) const
    {
        if (isNotValid())
            return;

        drawLine(R32(x1), R32(y1), R32(x2), R32(y2));
    }

    void RenderContext::drawLine(
        const R32 x1,
        const R32 y1,
        const R32 x2,
        const R32 y2) const
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
        _painter->drawText(x0, y0, QString::fromStdString(oss.str()));
    }

    void RenderContext::axisValue(
        const int  x0,
        const int  y0,
        const R32& v,
        const bool hor) const
    {
        OutputStringStream oss;
        oss << FloatPrint(v, 5, 4);
        _text = QString::fromStdString(oss.str());

        QRect r = _painter->fontMetrics()
                      .tightBoundingRect(_text)
                      .translated(x0, y0);

        if (hor)
            r = r.translated(-r.width() >> 1, r.height() << 1);
        else
            r = r.translated(0, r.height());

        _painter->drawText(r.x(), r.y(), _text);
    }

    void RenderContext::drawAxisF(
        const int   x0,
        const int   y0,
        const Axis& v) const
    {
        OutputStringStream oss;
        oss << SetS({"xn", "xd", "xn", "yd"})
            << Equ()
            << SetU({v.x.n(), v.x.d()})
            << SetU({v.y.n(), v.y.d()});
        _painter->drawText(x0, y0, QString(oss.str().c_str()));
    }

    void RenderContext::drawLines(const LineBuffer& buf) const
    {
        if (isNotValid())
            return;

        _painter->drawLines(buf.data(), buf.sizeI());
    }

}  // namespace Jam::Editor::State
