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
#include "Interface/Areas/GridAreaContent.h"
#include <QWidget>
#include "Interface/Extensions.h"
#include "Interface/Widgets/I32Widget.h"
#include "Interface/Widgets/StackedPanel.h"
#include "State/App.h"
#include "State/FrameStackManager.h"

namespace Jam::Editor
{
    using namespace State;

    GridAreaContent::GridAreaContent()
    {
        construct();
    }

    GridAreaContent::~GridAreaContent()
    {
        disconnectSignals();
    }

    void GridAreaContent::construct()
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

    void GridAreaContent::connectSignals()
    {
        connect(_y,
                &I32Widget::valueChanged,
                this,
                &GridAreaContent::yAxisUpdate);

        connect(_x,
                &I32Widget::valueChanged,
                this,
                &GridAreaContent::xAxisUpdate);

        //connect(layerStack(),
        //        &FrameStackManager::vec2Injected,
        //        this,
        //        &GridAreaContent::codeInjected);
    }

    void GridAreaContent::disconnectSignals()
    {
        //disconnect(layerStack(),
        //           &FrameStackManager::vec2Injected,
        //           this,
        //           &GridAreaContent::codeInjected);
    }

    //void GridAreaContent::codeInjected(const FrameStackCode& code,
    //                                      const Vec2F&          value) const
    //{
    //    const I32 v = I32(value.x);
    //    if (code == X_AXIS || code == X_STEP)
    //    {
    //        if (v != _x->value())
    //            _x->setValue(v);
    //    }
    //    else if (code == Y_AXIS || code == Y_STEP)
    //    {
    //        if (v != _y->value())
    //            _y->setValue(v);
    //    }
    //}

    void GridAreaContent::xAxisUpdate(const I32 v)
    {


        //layerStack()->scaleXAxis(R32(v));
    }

    void GridAreaContent::yAxisUpdate(const I32 v)
    {
        //layerStack()->scaleYAxis(R32(v));
    }

}  // namespace Jam::Editor