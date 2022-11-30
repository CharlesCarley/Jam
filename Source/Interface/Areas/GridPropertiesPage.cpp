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
#include "Interface/Areas/GridPropertiesPage.h"
#include <QWidget>
#include "Interface/Extensions.h"
#include "Interface/Widgets/I32Widget.h"
#include "Interface/Widgets/StackedPanel.h"
#include "State/App.h"
#include "State/FrameStack/GridLayer.h"
#include "State/FrameStackManager.h"

namespace Jam::Editor
{
    using namespace State;

    GridPropertiesPage::GridPropertiesPage(const U32 idx)
    {
        construct(idx);
    }

    GridPropertiesPage::~GridPropertiesPage()
    {
        disconnectSignals();
    }

    void GridPropertiesPage::construct(U32 idx)
    {
        View::applyColorRoles(this);

        const auto panel = new StackedPanel();
        panel->setLabel("Axis");

        _x = new I32Widget();
        _x->setLabel("x := ");
        _x->setValue(0);
        _x->setRange(-0x1000, 0x1000);

        _y = new I32Widget();
        _y->setLabel("y := ");
        _y->setValue(0);
        _y->setRange(-0x1000, 0x1000);

        panel->addWidget(_x);
        panel->addWidget(_y);

        addPanel(panel);
        connectSignals();
    }

    void GridPropertiesPage::connectSignals()
    {
        connect(_y,
                &I32Widget::valueChanged,
                this,
                &GridPropertiesPage::yAxisUpdate);

        connect(_x,
                &I32Widget::valueChanged,
                this,
                &GridPropertiesPage::xAxisUpdate);

        connect(State::layerStack(),
                &State::FrameStackManager::vec2Injected,
                this,
                &GridPropertiesPage::codeInjected);
    }

    void GridPropertiesPage::disconnectSignals()
    {
        disconnect(State::layerStack(),
                   &State::FrameStackManager::vec2Injected,
                   this,
                   &GridPropertiesPage::codeInjected);
    }

    void GridPropertiesPage::codeInjected(const FrameStackCode& code,
                                          const Vec2F&          value) const
    {
        const I32 v = I32(value.x);
        if (code == X_AXIS || code == X_STEP)
        {
            if (v != _x->value())
                _x->setValue(v);
        }
        else if (code == Y_AXIS || code == Y_STEP)
        {
            if (v != _y->value())
                _y->setValue(v);
        }
    }

    void GridPropertiesPage::xAxisUpdate(const I32 v)
    {
        if (const auto stack = State::layerStack())
            (void)stack->injectVec2(X_STEP, {R32(v), 0});
    }

    void GridPropertiesPage::yAxisUpdate(const I32 v)
    {
        if (const auto stack = State::layerStack())
            (void)stack->injectVec2(Y_STEP, {R32(v), 0});
    }

}  // namespace Jam::Editor