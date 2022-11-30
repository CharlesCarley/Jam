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
#include "FrameStackAreaContent.h"
#include <QMouseEvent>
#include "OutputArea.h"
#include "State/FrameStack/GridLayer.h"
#include "State/FrameStack/RenderContext.h"
#include "State/FrameStackManager.h"

namespace Jam::Editor
{
    using namespace State;

    FrameStackAreaContent::FrameStackAreaContent(QWidget* parent) :
        QWidget(parent)
    {
        construct();
    }

    FrameStackAreaContent::~FrameStackAreaContent()
    {
        if (const auto stack = State::layerStack())
        {
            disconnect(stack,
                       &State::FrameStackManager::vec2Injected,
                       this,
                       &FrameStackAreaContent::vec2Injected);
            disconnect(stack,
                       &State::FrameStackManager::stateChanged,
                       this,
                       &FrameStackAreaContent::stateChanged);
        }
    }

    void FrameStackAreaContent::construct()
    {
        setMinimumSize(I32(ScreenMin), I32(ScreenMin));
        setMaximumSize(I32(ScreenMax), I32(ScreenMax));

        setFocusPolicy(Qt::FocusPolicy::StrongFocus);

        connect(State::layerStack(),
                &State::FrameStackManager::vec2Injected,
                this,
                &FrameStackAreaContent::vec2Injected);
        connect(State::layerStack(),
                &State::FrameStackManager::stateChanged,
                this,
                &FrameStackAreaContent::stateChanged);

        resetAxis();
    }

    void FrameStackAreaContent::resetAxis()
    {
        const auto stack = State::layerStack();

        const QSize sz = size();

        constexpr R32 majorSubdivision = 1.f / 5.f;

        R32 minSquare = Min<R32>(
                            R32(sz.width()),
                            R32(sz.height())) *
                        majorSubdivision;

        _scrollX = minSquare;
        _scrollY = minSquare;
        _screen.init({0.f, 0.f});
        _screen.reset();

        (void)stack->injectVec2(X_AXIS, {minSquare, 1});
        (void)stack->injectVec2(Y_AXIS, {minSquare, 1});
        (void)stack->injectVec2(ORIGIN, _screen.origin());

        update();
    }

    Vec2F FrameStackAreaContent::updatePoint(
        const QMouseEvent* event)
    {
        const Vec2F p1 = {
            R32(event->position().x()),
            R32(event->position().y()),
        };
        const Vec2F p = {
            _p0.x - p1.x,
            _p0.y - p1.y,
        };
        _p0 = p1;
        return p;
    }

    void FrameStackAreaContent::updateSize(const QSize& sz)
    {
        _screen.setViewport(0, 0, sz.width(), sz.height());
        _screen.reset();

        (void)State::layerStack()->injectVec2(SIZE, _screen.viewport().extent());
        update();
    }

    void FrameStackAreaContent::paintEvent(QPaintEvent* event)
    {
        QPainter paint(this);
        paint.setRenderHint(QPainter::Antialiasing);

        RenderContext canvas(&paint, _screen);
        //canvas.setSize(Vec2I{(width()), (height())});
        canvas.clear(0x10, 0x10, 0x10, 0x80);

        if (const auto stk = State::layerStack())
            stk->render(&canvas);
    }

    void FrameStackAreaContent::resizeEvent(QResizeEvent* event)
    {
        updateSize(event->size());
    }

    void FrameStackAreaContent::wheelEvent(QWheelEvent* event)
    {
        const auto stack = State::layerStack();

        const R32 d = 6 * R32(event->angleDelta().y() > 0 ? 1 : -1);
        _scrollX += d;
        _scrollY += d;

        (void)stack->injectVec2(X_STEP, {_scrollX, 0.f});
        (void)stack->injectVec2(Y_STEP, {_scrollY, 0.f});

        update();
        event->accept();
    }

    void FrameStackAreaContent::mousePressEvent(QMouseEvent* event)
    {
        event->button() == Qt::LeftButton
            ? _state |= Left
            : _state &= ~Left;

        _p0 = {
            R32(event->position().x()),
            R32(event->position().y()),
        };

        QWidget::mousePressEvent(event);
    }

    void FrameStackAreaContent::mouseReleaseEvent(QMouseEvent* event)
    {
        event->button() == Qt::LeftButton
            ? _state |= Left
            : _state &= ~Left;

        QWidget::mouseReleaseEvent(event);
    }

    void FrameStackAreaContent::mouseMoveEvent(QMouseEvent* event)
    {
        if (_state & Left)
        {
            const Vec2F p = updatePoint(event);
            _screen.translate(-p.x, -p.y);

            if (State::layerStack()->injectVec2(ORIGIN, _screen.offset()))
                update();
        }
        QWidget::mouseMoveEvent(event);
    }

    void FrameStackAreaContent::keyPressEvent(QKeyEvent* event)
    {
        if (event->key() == Qt::Key_R)
            resetAxis();

        event->modifiers() ==
                Qt::ShiftModifier
            ? _state |= Shift
            : _state &= ~Shift;
    }

    void FrameStackAreaContent::keyReleaseEvent(QKeyEvent* event)
    {
        event->modifiers() ==
                Qt::ShiftModifier
            ? _state |= Shift
            : _state &= ~Shift;
    }

    void FrameStackAreaContent::vec2Injected(
        const FrameStackCode& code,
        const Vec2F&          vec)
    {
        if (code == X_STEP)
            _scrollX = vec.x;
        if (code == Y_STEP)
            _scrollY = vec.x;
        update();
    }

    void FrameStackAreaContent::stateChanged()
    {
        update();
    }
}  // namespace Jam::Editor
