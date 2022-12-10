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
#include <QPushButton>
#include <QResizeEvent>
#include "AreaType.h"
#include "FunctionAreaContent.h"
#include "Interface/Area/Area.h"
#include "Interface/Area/AreaToolbar.h"
#include "Interface/Extensions.h"
#include "Interface/Style/Style.h"
#include "Interface/Widgets/ExpressionWidget.h"
#include "Interface/Widgets/StackedPanel.h"
#include "Interface/Widgets/VariableWidget.h"
#include "Interface/Widgets/VerticalScrollArea.h"
#include "State/App.h"
#include "State/FrameStackManager.h"

namespace Jam::Editor
{
    FunctionArea::FunctionArea(AreaCreator* creator,
                               size_t       refId,
                               QWidget*     parent) :
        Area(creator, AtFunction, refId, parent)
    {
        _function = State::functionLayer();
        construct();
    }

    FunctionArea::~FunctionArea() = default;

    void FunctionArea::construct()
    {
        Style::apply(this, AreaStyle);

        const auto layout = Style::verticalLayout();
        _area = new VerticalScrollArea();
        constructContent();

        const auto tools = toolbar();
        constructTools(tools);

        layout->addWidget(tools);
        layout->addWidget(_area, 1);
        setLayout(layout);

        loadStack();
    }

    void FunctionArea::loadStack()
    {
        State::FunctionObjectArray foa;
        _function->findStateObjects(refId(), foa);

        for (const auto obj : foa)
        {
            if (obj->type() == State::FstVariable)
            {
                const auto vso = (State::VariableStateObject*)obj;
                auto       wid = _func->addSlider(vso);
                _variables.insert(vso->id(), wid);
            }
            else if (obj->type() == State::FstExpression)
            {
                const auto eso = (State::ExpressionStateObject*)obj;
                auto       wid = _func->addExpression(eso);
                _expressions.insert(eso->id(), wid);
            }
        }
    }

    void FunctionArea::constructTools(const AreaToolBar* tools)
    {
        const auto add = Style::toolButton(Icons::Menu);

        tools->addWidget(add, 0, Qt::AlignRight);

        connect(add, &QPushButton::clicked, this, [=]
                { displayOptions((QWidget*)sender()); });
    }

    void FunctionArea::constructContent()
    {
        _func = new FunctionAreaContent();
        _area->setWidget(_func);
        _area->invalidate();

        connect(_func,
                &FunctionAreaContent::contentChanged,
                this,
                &FunctionArea::onContentChanged);

        connect(_func,
                &FunctionAreaContent::deleteVariable,
                this,
                &FunctionArea::onDeleteVariable);
        connect(_func,
                &FunctionAreaContent::variableChanged,
                this,
                &FunctionArea::onVariableChanged);

        connect(_func,
                &FunctionAreaContent::deleteExpression,
                this,
                &FunctionArea::onDeleteExpression);

        connect(_func,
                &FunctionAreaContent::expressionChanged,
                this,
                &FunctionArea::onExpressionChanged);
    }

    void FunctionArea::displayOptions(QWidget* widget)
    {
        QMenu ctx(this);
        Style::apply(&ctx, AreaToolMenuItemStyle);

        ctx.addAction(get(Icons::Function), "Expression", [=]
                      { addExpression(); });
        ctx.addAction(get(Icons::Slider), "Slider", [=]
                      { addSlider(); });
        ctx.addAction(get(Icons::Point), "Point", [=]
                      { addPoint(); });

        widget->setFocus(Qt::FocusReason::PopupFocusReason);
        ctx.exec(mapToGlobal(widget->pos()));
    }

    template <typename W>
    void GenericRemove(HashTable<size_t, W*>& map,
                       size_t                 refid,
                       const W*               widget)
    {
        if (const size_t n = map.find(refid); n != JtNpos)
        {
            if (map.at(n) != widget)
            {
                // see See AreaTest->eventTest2
                // It no longer refers to a valid reference.
                // In this case, remove it from this side,
                // and delete the widget as normal

                // throw Exception("invalid state");
                delete widget;
                map.erase(refid);
                return;
            }

            map.erase(refid);
            delete widget;
#ifdef _DEBUG
            if (const size_t m = map.find(refid);
                m != JtNpos)
                throw Exception("invalid state");  // would be a bug.
#endif
        }
        else
        {
            // possible causes:
            // 1. It was not added properly.
            // 2. There is a bug in find
            // But, widget was dangled and must be deleted
            if (!widget)
                throw Exception("invalid state");
            delete widget;
        }
    }

    void FunctionArea::onDeleteVariable(const size_t refid, const VariableWidget* widget)
    {
        _function->removeVariable(refid);
        GenericRemove(_variables, refid, widget);
        _area->invalidate();
        State::layerStack()->notifyStateChange(this);
    }

    void FunctionArea::onDeleteExpression(const size_t refid, const ExpressionWidget* widget)
    {
        // remove it from the function layer
        _function->removeExpression(refid);
        GenericRemove(_expressions, refid, widget);
        _area->invalidate();
        State::layerStack()->notifyStateChange(this);
    }

    void FunctionArea::onVariableChanged(const size_t refid, const VariableStepData& data) const
    {
        if (const size_t n = _variables.find(refid);
            n != JtNpos)
        {
            if (const auto var = _function->findVariable(refid))
            {
                var->setValue(data.value);
                var->setRange(data.range);
                var->setName(data.name);
                var->setRate(data.rate);

                _function->update();
                State::layerStack()->notifyStateChange(this);
            }
        }
    }

    void FunctionArea::onExpressionChanged(const size_t refid, const String& text) const
    {
        if (const size_t n = _expressions.find(refid);
            n != JtNpos)
        {
            if (const auto exp = _function->findExpression(refid))
            {
                exp->setText(text);
                _function->update();
                State::layerStack()->notifyStateChange(this);
            }
        }
    }

    void FunctionArea::onContentChanged() const
    {
        _area->invalidate();
    }

    VariableWidget* FunctionArea::addSlider()
    {
        auto var = _function->createVariable(refId());
        auto wid = _func->addSlider(var);

        _variables.insert(var->id(), wid);

        _area->invalidate();
        // pair.second -> links back via deleteVariable
        return wid;
    }

    ExpressionWidget* FunctionArea::addExpression()
    {
        auto var = _function->createExpression(refId());
        auto wid = _func->addExpression(var);
        _expressions.insert(var->id(), wid);

        _area->invalidate();
        // pair.second -> links back via deleteExpression
        return wid;
    }

    void FunctionArea::addPoint() const
    {
        if (_func)
        {
            _func->addPoint();
            _area->invalidate();
        }
    }

}  // namespace Jam::Editor
