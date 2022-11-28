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
#include "Interface/Areas/FunctionArea.h"
#include <qslider.h>
#include <qtextformat.h>
#include "FunctionPropertiesPage.h"
#include "Interface/Area/Area.h"
#include "Interface/Area/AreaToolbar.h"
#include "Interface/Constants.h"
#include "Interface/Events/LayerSelectEvent.h"
#include "Interface/Extensions.h"
#include "Interface/Widgets/StackedPanel.h"

namespace Jam::Editor
{

    FunctionArea::FunctionArea(AreaCreator* creator, QWidget* parent) :
        Area(creator, AtFunction, parent)
    {
        construct();
    }

    FunctionArea::~FunctionArea() = default;

    void FunctionArea::construct()
    {
        View::widgetDefaults(this);
        View::applyColorRoles(this, Const::SplitterRole);

        const auto layout = new QVBoxLayout();
        View::layoutDefaults(layout);

        _area = new QScrollArea();
        _area->setAlignment(Qt::AlignRight);
        _area->setWidgetResizable(true);

        layout->addWidget(toolbar());
        layout->addWidget(_area, 1);

        _func = new FunctionPropertiesPage(1);
        _area->setWidget(_func);

        setLayout(layout);
    }

    bool FunctionArea::event(QEvent* event)
    {
        switch ((AreaEvents)event->type())
        {
        case ProjectClosed:
            _func->clear();
            break;
        case ProjectChanged:
        case ProjectOpened:
            _func = new FunctionPropertiesPage(1);
            _area->setWidget(_func);
            break;
        case LayerSelect:
        case SplitEvent:
        case MergeLeftEvent:
        case MergeRightEvent:
        case SwitchContentEvent:
        default:
            break;
        }
        return Area::event(event);
    }

}  // namespace Jam::Editor
