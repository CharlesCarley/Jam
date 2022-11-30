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
#include "Interface/Areas/FunctionPropertiesPage.h"
#include <QApplication>
#include <QResizeEvent>
#include <QWidget>
#include "Interface/Extensions.h"
#include "Interface/Widgets/ExpressionWidget.h"
#include "Interface/Widgets/R32Widget.h"
#include "Interface/Widgets/StackedPanel.h"
#include "Interface/Widgets/StringWidget.h"
#include "Interface/Widgets/VariableWidget.h"
#include "State/App.h"
#include "State/FrameStack/FunctionLayer.h"
#include "State/FrameStackManager.h"

namespace Jam::Editor
{
    FunctionPropertiesPage::FunctionPropertiesPage()
    {
        construct();
    }

    FunctionPropertiesPage::~FunctionPropertiesPage() = default;

    void FunctionPropertiesPage::construct()
    {
        View::applyColorRoles(this);
        _panel = new StackedPanel();
        _panel->setLabel("Functions");
        loadState();
        addPanel(_panel);
    }

    void FunctionPropertiesPage::loadState() const
    {
        const auto layer = State::functionLayer();

        for (const auto object : layer->objects())
        {
            if (object->type == State::FstExpression)
                addExpression((State::ExpressionStateObject*)object);
            if (object->type == State::FstVariable)
                addSlider((State::VariableStateObject*)object);
        }
    }

    void FunctionPropertiesPage::addSlider(State::VariableStateObject* obj) const
    {
        if (obj == nullptr)
            obj = State::functionLayer()->createVariable();

        if (obj == nullptr)  // unlikely
            throw Exception("invalid state object");

        const auto var = new VariableWidget(obj);
        connect(var,
                &VariableWidget::wantsToDelete,
                this,
                [=]
                { dropWidget((QWidget*)sender()); });

        _panel->addWidget(var);
        notifyResize();
    }

    void FunctionPropertiesPage::addExpression(State::ExpressionStateObject* obj) const
    {
        if (obj == nullptr)
            obj = State::functionLayer()->createExpression();

        if (obj == nullptr)  // unlikely
            throw Exception("invalid state object");

        const auto exp = new ExpressionWidget(obj);
        connect(exp,
                &ExpressionWidget::wantsToDelete,
                this,
                [=]
                { dropWidget((QWidget*)sender()); });

        _panel->addWidget(exp);
        notifyResize();
    }

    void FunctionPropertiesPage::addPoint() const
    {
        const auto str = new StringWidget();
        _panel->addWidget(str);
        notifyResize();
    }

    void FunctionPropertiesPage::notifyResize() const
    {
        if (QWidget* parent = parentWidget())
        {
            const QSize sz = parent->size();
            QApplication::postEvent(
                parent,
                new QResizeEvent(sz, _panel->size()));
        }
    }

    void FunctionPropertiesPage::dropWidget(QWidget* widget) const
    {
        if (widget)
        {
            _panel->remove(widget);
            delete widget;
            notifyResize();
        }
    }

    void FunctionPropertiesPage::onTextChanged(const String& newText)
    {
        if (State::layerStack()->injectText(newText))
            update();
    }

}  // namespace Jam::Editor