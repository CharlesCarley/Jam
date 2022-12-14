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
#include <QPainter>
#include <QPalette>
#include "Math/Axis.h"
#include "Math/Color.h"
#include "Math/Screen.h"
#include "Math/Vec2.h"

namespace Jam::Editor::State
{
    using LineBuffer = SimpleArray<QLineF>;
    using RealBuffer = SimpleArray<Vec2F>;

    class RenderContext
    {
    private:
        IColor          _color{0, 0, 0, 1};
        Vec2I           _size{U32(0), 0};
        Screen          _screen;
        QPainter*       _painter;
        QPen            _pen{};
        const QPalette* _palette;

        mutable QString _text;

        bool isNotValid() const;

    public:
        explicit RenderContext(QPainter*       painter,
                               const QPalette* palette,
                               Screen          screen);
        ~RenderContext();

        const Vec2I& size() const;

        void axisValue(const R32 x0, const R32 y0, const R32& v2, const bool hor = true) const;

        void selectColor(const QPalette::ColorRole& col, U8 width = 1);

        void selectColor(const U32& col, U8 width = 1);

        void selectColor(const Color& col, U8 width = 1);

        void selectColor(const QColor& col, U8 width = 1);

        void drawLine(int x1, int y1, int x2, int y2) const;

        void drawLine(R32 x1, R32 y1, R32 x2, R32 y2) const;

        void drawPoint(int x0, int y0) const;

        void drawPoint(int x0, int y0, int scale) const;

        void drawR32(int x0, int y0, const String& s, const R32& v, U8 p = 3) const;

        void drawVec2F(int x0, int y0, const Vec2F& v, U8 p = 3) const;

        void drawBoxF(const Box& v, U8 p = 3) const;

        void drawAxisF(int x0, int y0, const Axis& v) const;

        void drawLines(const LineBuffer& buf) const;

        void copyBuffer(void* src, U32 w, U32 h) const;

        void clear(U8 red,
                   U8 green,
                   U8 blue,
                   U8 alpha) const;
    };

    inline const Vec2I& RenderContext::size() const
    {
        return _size;
    }

    inline bool RenderContext::isNotValid() const
    {
        return _painter == nullptr;
    }

}  // namespace Jam::Editor::State
