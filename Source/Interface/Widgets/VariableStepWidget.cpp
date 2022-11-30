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
#include "VariableStepWidget.h"
#include <qevent.h>
#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>
#include "Interface/Areas/OutputArea.h"
#include "Interface/Extensions.h"
#include "Utils/Char.h"

namespace Jam::Editor
{

    VariableStepWidget::VariableStepWidget(QWidget* parent) :
        QWidget(parent)
    {
        construct();
    }

    void VariableStepWidget::construct()
    {
        View::emptyWidget(this);

        const auto layout = new QHBoxLayout();
        View::layoutDefaults(layout, 0, 6);

        const auto l1 = new QLabel("Name:");
        View::localDefaults(l1, 0);
        _name = new QLineEdit();
        View::lineEditDefaults(_name, QPalette::Base);

        const auto l2 = new QLabel("Min:");
        View::localDefaults(l2, 0);
        _min = new QLineEdit();
        View::lineEditDefaults(_min);

        const auto l3 = new QLabel("Max:");
        View::localDefaults(l3, 0);
        _max = new QLineEdit();
        View::lineEditDefaults(_max);

        const auto l4 = new QLabel("Step:");
        View::localDefaults(l4, 0);
        _rate = new QLineEdit();
        View::lineEditDefaults(_rate);

        layout->addWidget(l1, 0);
        layout->addWidget(_name, 1);
        layout->addWidget(l2, 0);
        layout->addWidget(_min, 1);
        layout->addWidget(l3, 0);
        layout->addWidget(_max, 1);
        layout->addWidget(l4, 0);
        layout->addWidget(_rate, 1);
        setLayout(layout);

        connectSignals();
    }

    void VariableStepWidget::connectSignals()
    {
        connect(_name, &QLineEdit::editingFinished, this, &VariableStepWidget::onTextChanged);
        connect(_min, &QLineEdit::editingFinished, this, &VariableStepWidget::onTextChanged);
        connect(_max, &QLineEdit::editingFinished, this, &VariableStepWidget::onTextChanged);
        connect(_rate, &QLineEdit::editingFinished, this, &VariableStepWidget::onTextChanged);
    }

    void VariableStepWidget::onTextChanged()
    {
        //Log::writeLine("text-out");
        if (sender() == _name)
        {
            const String s = _name->text().toStdString();
            (void)Su::filterRange(_data.name, s, 'a', 'z');
            setFocus();
        }
        else if (sender() == _min)
        {
            const String s = _min->text().toStdString();
            String       v;
            Su::filterReal(v, s);
            _data.range.x = Char::toFloat(v);
            setFocus();
        }
        else if (sender() == _max)
        {
            const String s = _max->text().toStdString();
            String       v;
            Su::filterReal(v, s);
            _data.range.y = Char::toFloat(v);
            setFocus();
        }
        else if (sender() == _rate)
        {
            const String s = _rate->text().toStdString();
            String       v;
            Su::filterReal(v, s);
            _data.rate = Char::toFloat(v);
            setFocus();
        }
    }

    void VariableStepWidget::focusOutEvent(QFocusEvent* event)
    {
        QWidget::focusOutEvent(event);

        if (event->lostFocus())
        {
            if (!_name->hasFocus() &&
                !_min->hasFocus() &&
                !_max->hasFocus() &&
                !_rate->hasFocus())
            {
                emit finished(_data);
                //Log::writeLine("finished");
            }
            //Log::writeLine("focus-out");
        }
    }

    void VariableStepWidget::focusInEvent(QFocusEvent* event)
    {
        //Log::writeLine("focus-in");
        QWidget::focusInEvent(event);
    }

    void VariableStepWidget::setName(const String& value)
    {
        _name->setText(QString::fromStdString(value));
        _data.name = value;
    }

    void VariableStepWidget::setRange(const Vec2F& value)
    {
        _min->setText(QString::fromStdString(Char::toString(value.x)));
        _max->setText(QString::fromStdString(Char::toString(value.y)));
        _data.range = value;
    }

    void VariableStepWidget::setRate(const R32& value)
    {
        _rate->setText(QString::fromStdString(Char::toString(value)));
        _data.rate = value;
    }

}  // namespace Jam::Editor
