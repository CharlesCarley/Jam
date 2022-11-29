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
#include <QWidget>
#include "Math/Screen.h"
#include "State/FrameStack/GridLayer.h"

namespace Jam::Editor
{
    class FrameStackAreaPrivate final : public QWidget
    {
        Q_OBJECT
    private:
        enum InputStates
        {
            Left  = 0x01,
            Shift = 0x02,
        };
        // LeftPressed = 0x01
        // Shift       = 0x02
        U32    _state{0};
        Screen _screen;
        Vec2F  _p0;
        R32    _scrollX{0};
        R32    _scrollY{0};

    public:
        explicit FrameStackAreaPrivate(QWidget* parent = nullptr);
        ~FrameStackAreaPrivate() override;

        void resetAxis();


    private:
        void  construct();
        Vec2F updatePoint(const QMouseEvent* event);

        void updateSize(const QSize& sz);

        void paintEvent(QPaintEvent* event) override;

        void resizeEvent(QResizeEvent* event) override;

        void wheelEvent(QWheelEvent* event) override;

        void mousePressEvent(QMouseEvent* event) override;

        void mouseReleaseEvent(QMouseEvent* event) override;

        void mouseMoveEvent(QMouseEvent* event) override;

        void keyPressEvent(QKeyEvent* event) override;

        void keyReleaseEvent(QKeyEvent* event) override;

        void vec2Injected(const FrameStackCode& code, const Vec2F& vec);

        void stateChanged();
    };
}  // namespace Jam::Editor
