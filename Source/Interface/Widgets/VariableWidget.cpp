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
#include "Interface/Widgets/VariableWidget.h"
#include <QBoxLayout>
#include "IconButton.h"
#include "Interface/Extensions.h"
#include "Interface/Widgets/R32Widget.h"

namespace Jam::Editor
{

    VariableWidget::VariableWidget(QWidget* parent) :
        QWidget(parent)
    {
        construct();
    }

    void VariableWidget::construct()
    {
        View::widgetDefaults(this);
        View::buttonDefaults(this);

        const auto layout = new QHBoxLayout();
        View::layoutDefaults(layout);

        _line = new R32Widget();
        _del  = IconButton::create(Icons::Delete);
        View::copyColorRoles(_del, this);

        layout->addWidget(_line, 1);
        layout->addWidget(_del);
        setLayout(layout);

        connectSignals();
    }

    void VariableWidget::connectSignals()
    {
        connect(_del,
                &QPushButton::clicked,
                this,
                [=]
                {
                    emit deleteVariable(_refId);
                });
        connect(_line,
                &R32Widget::stepDataChanged,
                this,
                &VariableWidget::onStepDataChanged);
        connect(_line,
                &R32Widget::valueChanged,
                this,
                &VariableWidget::onValueChanged);
    }

    void VariableWidget::onStepDataChanged(const VariableStepData& data)
    {
        _stepData = data;
        _line->setValue(_stepData.value);
        emit variableChanged(_refId, _stepData);
    }

    void VariableWidget::onValueChanged(const R32& data)
    {
        _stepData.value = data;
        emit variableChanged(_refId, _stepData);
    }

    void VariableWidget::setName(const String& name)
    {
        _stepData.name = name;
        _line->setLabel(name);
        emit variableChanged(_refId, _stepData);
    }

    void VariableWidget::setRange(const Vec2F& range)
    {
        _stepData.range = range;
        _line->setRange(range.x, range.y);
        emit variableChanged(_refId, _stepData);
    }

    void VariableWidget::setRate(const R32& rate)
    {
        _stepData.rate = rate;
        _line->setRate(rate);
        emit variableChanged(_refId, _stepData);
    }

    void VariableWidget::setValue(const R32& value)
    {
        _stepData.value= value;
        _line->setValue(value);
        emit variableChanged(_refId, _stepData);
    }

}  // namespace Jam::Editor
