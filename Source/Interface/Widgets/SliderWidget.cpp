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
#include "SliderWidget.h"
#include <qtextlayout.h>
#include <QBoxLayout>
#include <QWidget>
#include "Interface/Areas/OutputArea.h"
#include "Interface/Style/Style.h"
#include "SliderEditWidget.h"
#include "SliderSlideWidget.h"

namespace Jam::Editor
{

    SliderWidget::SliderWidget(QWidget* parent) :
        QWidget(parent)
    {
        construct();
    }

    SliderWidget::~SliderWidget()
    {
        delete _layout;
        delete _value;
        delete _step;
    }

    void SliderWidget::construct()
    {
        Style::apply(this, AreaSliderStyle);

        _layout = Style::horizontalLayout();
        _value  = new SliderSlideWidget();

        _step = new SliderEditWidget();
        _step->setVisible(false);

        _layout->addWidget(_value);
        _layout->addWidget(_step);

        setLayout(_layout);

        connectSignals();
    }

    void SliderWidget::connectSignals()
    {
        if (_step)
        {
            connect(_step,
                    &SliderEditWidget::stepParamChange,
                    this,
                    &SliderWidget::onStepParamChange);
        }

        if (_value)
        {
            connect(_value,
                    &SliderSlideWidget::doubleClicked,
                    this,
                    &SliderWidget::onDoubleClicked);
            connect(_value,
                    &SliderSlideWidget::valueChanged,
                    this,
                    &SliderWidget::onValueChanged);
        }
    }

    void SliderWidget::setValue(const R32& value)
    {
        _data.value = value;
        if (_value)
            _value->setValue(_data.value);
    }

    void SliderWidget::setRange(const R32& min, const R32& max)
    {
        _data.range = {min, max};
        if (_value)
            _value->setRange(_data.range);
    }

    void SliderWidget::setRate(const R32& rate)
    {
        _data.rate = rate;
        if (_value)
            _value->setRate(_data.rate);
    }

    void SliderWidget::setLabel(const String& label)
    {
        Su::filterAZaz(_data.name, label, 1);
        if (_value)
            _value->setLabel(_data.name);
    }

    R32 SliderWidget::value() const
    {
        return _value ? _value->value() : _data.value;
    }

    Vec2F SliderWidget::range() const
    {
        return _value ? _value->range() : _data.range;
    }

    String SliderWidget::string() const
    {
        return _value ? _value->text() : "";
    }

    String SliderWidget::label() const
    {
        return _value ? _value->label() : "";
    }

    void SliderWidget::editingFinished() const
    {
        makeEditable(false);
    }

    void SliderWidget::onDoubleClicked() const
    {
        makeEditable(true);
    }

    void SliderWidget::returnPressed() const
    {
        makeEditable(false);
    }

    void SliderWidget::onStepParamChange(const VariableStepData& data)
    {
        makeEditable(false);
        _data = data;
        _value->setStepData(_data);
        emit stepDataChanged(data);
    }

    void SliderWidget::onValueChanged(const R32& value)
    {
        _data.value = value;
        emit stepDataChanged(_data);
    }

    void SliderWidget::makeEditable(const bool edit) const
    {
        if (!_value || !_step)
            return;

        _value->setVisible(!edit);
        _step->setVisible(edit);

        if (edit)
        {
            _step->setStepData(_data);
            _step->setFocus(Qt::MouseFocusReason);
            _value->setFocus(Qt::NoFocusReason);
        }
        else
        {
            _step->setFocus(Qt::NoFocusReason);
            _value->setFocus(Qt::MouseFocusReason);
        }
    }

    void SliderWidget::mouseDoubleClickEvent(QMouseEvent* event)
    {
        if (!event)
            return;
        onDoubleClicked();
        event->accept();
    }

    void SliderWidget::keyPressEvent(QKeyEvent* event)
    {
        if (!event)
            return;

        if (event->key() == Qt::Key_Escape)
        {
            makeEditable(false);
            event->accept();
        }
        else
            QWidget::keyPressEvent(event);
    }

}  // namespace Jam::Editor
