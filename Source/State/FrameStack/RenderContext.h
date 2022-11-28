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
#include "Math/Axis.h"
#include "Math/Color.h"
#include "Math/Integer.h"
#include "Math/Screen.h"
#include "Math/Vec2.h"

namespace Jam
{
    using LineBuffer = SimpleArray<QLineF>;

    class RenderContext
    {
    private:
        IColor    _color{0, 0, 0, 1};
        Vec2I     _size{U32(0), 0};
        Screen    _screen;
        QPainter* _painter;
        QPen      _pen{};

        LineBuffer _major;
        LineBuffer _minor;
        LineBuffer _center;
        
        void stepGrid(const R32    x1,
                      const R32    y1,
                      const R32    x2,
                      const R32    y2,
                      const Vec2F& ax,
                      const Vec2F& offset,
                      LineBuffer&  buffer) const;
        void stepLabels(const R32    x1,
                        const R32    y1,
                        const R32    x2,
                        const R32    y2,
                        const Vec2F& ax,
                        const Vec2F& offset,
                        const Axis&  gx) const;

        void axisLine(const R32&  step,
                      int         dir,
                      LineBuffer& dest) const;

        bool isNotValid() const;

        void axisValue(int x0, int y0, const R32& v, bool hor = true) const;

    public:
        explicit RenderContext(QPainter* painter,
                               Screen    screen);
        ~RenderContext();

        const Vec2I& size() const;

        void setSize(const Vec2I& size);

        void selectColor(const U32& col, int w = 1);

        void selectColor(const Color& col, int w = 1);

        void selectColor(const QColor& col, int w = 1);

        void drawLine(int x1, int y1, int x2, int y2) const;

        void drawLine(R32 x1, R32 y1, R32 x2, R32 y2) const;

        void drawPoint(int x0, int y0) const;

        void drawPoint(int x0, int y0, int scale) const;

        void drawVec2F(int x0, int y0, const Vec2F& v, U8 p = 3) const;
        
        void drawAxisF(int x0, int y0, const Axis& v) const;

        void screenGrid(
            const Axis& axis,
            const U32&  majorColor,
            const U32&  minorColor,
            const U32&  centerColor,
            const U32&  textColor);

        void copyBuffer(void* src, U32 w, U32 h) const;

        void clear(U8 red,
                   U8 green,
                   U8 blue,
                   U8 alpha) const;

        void flush() const;
    };

    inline const Vec2I& RenderContext::size() const
    {
        return _size;
    }

    inline void RenderContext::setSize(const Vec2I& size)
    {
        _size = size;
    }

    inline bool RenderContext::isNotValid() const
    {
        return _painter == nullptr;
    }

}  // namespace Jam
