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
#include <QMenu>
#include <QResizeEvent>
#include "AreaType.h"
#include "FunctionAreaContent.h"
#include "Interface/Area/Area.h"
#include "Interface/Area/AreaToolbar.h"
#include "Interface/Constants.h"
#include "Interface/Events/LayerSelectEvent.h"
#include "Interface/Extensions.h"
#include "Interface/Widgets/IconButton.h"
#include "Interface/Widgets/StackedPanel.h"
#include "Interface/Widgets/VerticalScrollArea.h"

namespace Jam::Editor
{
    using namespace State;

    FunctionArea::FunctionArea(AreaCreator* creator,
                               QWidget*     parent) :
        Area(creator, AtFunction, parent)
    {
        construct();
    }

    FunctionArea::~FunctionArea() = default;

    void FunctionArea::construct()
    {
        View::widgetDefaults(this);
        View::applyColorRoles(this, Const::WindowRole);

        const auto layout = new QVBoxLayout();
        View::layoutDefaults(layout);

        _area = new VerticalScrollArea();
        _func = new FunctionAreaContent();
        _area->setWidget(_func);

        const auto tools = toolbar();
        constructTools(tools);

        layout->addWidget(tools);
        layout->addWidget(_area, 1);
        setLayout(layout);
    }

    void FunctionArea::constructTools(const AreaToolBar* tools)
    {
        const auto add = IconButton::createToolButton(Icons::Menu);
        tools->addWidget(
            add,
            0,
            Qt::AlignRight);

        connect(add, &QPushButton::clicked, this, [=]
                { displayOptions((QWidget*)sender()); });
    }

    void FunctionArea::displayOptions(QWidget* widget)
    {
        QMenu ctx(this);
        ctx.addAction(get(Icons::Function), "Expression", [=]
                      { addExpression(); });
        ctx.addAction(get(Icons::Slider), "Slider", [=]
                      { addSlider(); });
        ctx.addAction(get(Icons::Point), "Point", [=]
                      { addPoint(); });

        widget->setFocus(Qt::FocusReason::PopupFocusReason);
        ctx.exec(mapToGlobal(widget->pos()));
    }

    void FunctionArea::addSlider() const
    {
        if (_func)
            _func->addSlider();
    }

    void FunctionArea::addExpression() const
    {
        if (_func)
            _func->addExpression();
    }

    void FunctionArea::addPoint() const
    {
        if (_func)
            _func->addPoint();
    }

    bool FunctionArea::event(QEvent* event)
    {
        switch ((AreaEvents)event->type())
        {
        case ProjectOpened:
            _func = new FunctionAreaContent();
            _area->setWidget(_func);
            break;
        case ProjectClosed:
        case LayerSelect:
        case SplitEvent:
        case MergeLeftEvent:
        case MergeRightEvent:
        case SwitchContentEvent:
            break;
        }
        return Area::event(event);
    }

}  // namespace Jam::Editor
