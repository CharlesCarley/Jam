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
