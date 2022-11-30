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
#include "R32Widget.h"
#include <qtextlayout.h>
#include <QBoxLayout>
#include <QWidget>
#include "Interface/Areas/OutputArea.h"
#include "Interface/Extensions.h"
#include "R32WidgetPrivate.h"
#include "VariableStepWidget.h"

namespace Jam::Editor
{

    R32Widget::R32Widget(QWidget* parent) :
        QWidget(parent)
    {
        construct();
    }

    R32Widget::~R32Widget()
    {
        delete _layout;
        delete _value;
        delete _step;
    }

    void R32Widget::construct()
    {
        View::applyColorRoles(this);

        _layout = new QHBoxLayout();
        View::layoutDefaults(_layout);

        _value = new R32WidgetPrivate();
        View::applyColorRoles(_value);

        _step = new VariableStepWidget();
        View::applyColorRoles(_step);
        _step->setVisible(false);

        _layout->addWidget(_value);
        _layout->addWidget(_step);

        connectEvents();
        setLayout(_layout);
    }

    void R32Widget::editingFinished() const
    {
        makeEditable(false);
    }

    void R32Widget::doubleClicked() const
    {
        makeEditable(true);
    }

    void R32Widget::returnPressed() const
    {
        makeEditable(false);
    }

    void R32Widget::onStepParamChange(const VariableStepData& dt) const
    {
        makeEditable(false);
        setLabel(dt.name);
        setRange(dt.range.x, dt.range.y);

        emit stepDataChanged(dt);
    }

    void R32Widget::makeEditable(const bool edit) const
    {
        if (!_value || !_step)
            return;

        _value->setVisible(!edit);
        _step->setVisible(edit);

        if (edit)
        {
            _step->setFocus(Qt::MouseFocusReason);
            _value->setFocus(Qt::NoFocusReason);
        }
        else
        {
            _step->setFocus(Qt::NoFocusReason);
            _value->setFocus(Qt::MouseFocusReason);
        }
    }

    void R32Widget::connectEvents()
    {
        if (_step)
        {
            connect(_step,
                    &VariableStepWidget::finished,
                    this,
                    &R32Widget::onStepParamChange);
        }

        if (_value)
        {
            connect(_value,
                    &R32WidgetPrivate::onDoubleClick,
                    this,
                    &R32Widget::doubleClicked);

            connect(_value,
                    &R32WidgetPrivate::valueChanged,
                    this,
                    [this](const R32 v)
                    { emit valueChanged(v); });
        }
    }

    void R32Widget::mouseDoubleClickEvent(QMouseEvent* event)
    {
        if (!event)
            return;
        doubleClicked();
        event->accept();
    }

    void R32Widget::keyPressEvent(QKeyEvent* event)
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

    void R32Widget::setValue(const R32& value) const
    {
        if (_value)
            _value->setValue(value);
    }

    void R32Widget::setRange(const R32& min, const R32& max) const
    {
        if (_value)
            _value->setRange({min, max});
        if (_step)
            _step->setRange({min, max});
    }

    void R32Widget::setLabel(const String& value) const
    {
        if (_value)
            _value->setLabel(Su::join(value, " := "));

        if (_step)
            _step->setName(value);
    }

    R32 R32Widget::value() const
    {
        return _value ? _value->value() : -1;
    }

    String R32Widget::string() const
    {
        return _value ? _value->text() : "";
    }

    String R32Widget::label() const
    {
        if (_value)
            return _value->label();
        return {};
    }

    void R32Widget::setHeight(int hgt) const
    {
        _value->setMinimumHeight(hgt);
        _step->setMinimumHeight(hgt);
    }
}  // namespace Jam::Editor
