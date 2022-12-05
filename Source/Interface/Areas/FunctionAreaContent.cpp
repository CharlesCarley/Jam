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
#include "Interface/Areas/FunctionAreaContent.h"
#include <QApplication>
#include <QResizeEvent>
#include <QWidget>

#include "Interface/Style/Style.h"
#include "Interface/Widgets/ExpressionWidget.h"
#include "Interface/Widgets/StackedPanel.h"
#include "Interface/Widgets/VariableWidget.h"
#include "State/App.h"
#include "State/FrameStack/FunctionLayer.h"
#include "State/FrameStackManager.h"

namespace Jam::Editor
{
    FunctionAreaContent::FunctionAreaContent()
    {
        construct();
    }

    FunctionAreaContent::~FunctionAreaContent() = default;

    void FunctionAreaContent::construct()
    {
        Style::apply(this, TransparentStyle);

        _panel = new StackedPanel();
        // _panel->setLabel("Functions");
        addPanel(_panel);
    }

    VariableWidget* FunctionAreaContent::addSlider(
        const State::VariableStateObject* obj) const
    {
        const auto var = new VariableWidget();
        _panel->addWidget(var);
        if (obj)
        {
            var->setRefId(obj->id());
            var->setRange(obj->range());
            var->setValue(obj->value());
            var->setRate(obj->rate());
            var->setName(obj->name());
        }

        connect(var,
                &VariableWidget::deleteVariable,
                this,
                &FunctionAreaContent::onDeleteVariable);
        connect(var,
                &VariableWidget::variableChanged,
                this,
                &FunctionAreaContent::variableChanged);

        return var;
    }

    ExpressionWidget* FunctionAreaContent::addExpression(
        const State::ExpressionStateObject* obj) const
    {
        const auto exp = new ExpressionWidget();
        _panel->addWidget(exp);
        if (obj)
        {
            exp->setRefId(obj->id());
            exp->setText(obj->text());
        }

        connect(exp,
                &ExpressionWidget::deleteExpression,
                this,
                &FunctionAreaContent::onDeleteExpression);
        connect(exp,
                &ExpressionWidget::expressionChanged,
                this,
                &FunctionAreaContent::expressionChanged);
        return exp;
    }

    void FunctionAreaContent::addPoint() const
    {
        // TODO addPoint
        // addExpression();
    }

    void FunctionAreaContent::onDeleteGeneral() const
    {
        if (QObject* obj = sender();
            obj && obj->isWidgetType())
        {
            QWidget* widget = (QWidget*)obj;
            // remove it from the panel now, but delay delete it
            widget->setParent(nullptr);  // detached from tree
            _panel->remove(widget);      // widget not deleted
        }
    }

    void FunctionAreaContent::onDeleteVariable(const size_t refid)
    {
        // This must be linked back to
        // FunctionArea::onDeleteVariable
        onDeleteGeneral();
        emit deleteVariable(refid, (VariableWidget*)sender());
    }

    void FunctionAreaContent::onDeleteExpression(const size_t refid)
    {
        // This must be linked back to
        // FunctionArea::onDeleteExpression
        onDeleteGeneral();
        emit deleteExpression(refid, (ExpressionWidget*)sender());
    }

}  // namespace Jam::Editor