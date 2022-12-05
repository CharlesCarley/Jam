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

    GridAreaContent::~GridAreaContent() = default;

    void GridAreaContent::construct()
    {
        Style::apply(this, TransparentStyle);


        const auto panel = new StackedPanel();

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
    }

    void GridAreaContent::xAxisUpdate(const I32 v)
    {
    }

    void GridAreaContent::yAxisUpdate(const I32 v)
    {
    }

}  // namespace Jam::Editor