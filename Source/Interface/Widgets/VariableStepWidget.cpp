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
#include "Interface/Constants.h"
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
        View::buttonDefaults(this);

        const auto layout = new QHBoxLayout();
        View::layoutDefaults(layout, 2, 1);

        makePair(_name, "id");
        makePair(_value, ":=");
        makePair(_min, ">=");
        makePair(_max, "<=");
        makePair(_rate, "+=");

        layout->addWidget(_name.first, 0);
        layout->addWidget(_name.second, 1);

        layout->addWidget(_value.first, 0);
        layout->addWidget(_value.second, 1);

        layout->addWidget(_min.first, 0);
        layout->addWidget(_min.second, 1);

        layout->addWidget(_max.first, 0);
        layout->addWidget(_max.second, 1);

        layout->addWidget(_rate.first, 0);
        layout->addWidget(_rate.second, 1);

        setLayout(layout);
    }

    bool VariableStepWidget::anyFocused() const
    {
        return _rate.second->hasFocus() ||
               _name.second->hasFocus() ||
               _min.second->hasFocus() ||
               _max.second->hasFocus() ||
               _value.second->hasFocus();
    }

    void VariableStepWidget::makePair(
        LabelLineEditPair& dest,
        const char*        name)
    {
        dest.first = new QLabel(name);
        View::localDefaults(dest.first, 0);
        View::buttonDefaults(dest.first);
        dest.first->setFocusPolicy(Qt::NoFocus);

        dest.second = new QLineEdit();
        View::lineEditDefaults(dest.second, QPalette::Base);
        dest.second->setFocusPolicy(Qt::StrongFocus);

        connect(dest.second,
                &QLineEdit::editingFinished,
                this,
                &VariableStepWidget::onTextChanged);
        connect(dest.second,
                &QLineEdit::returnPressed,
                this,
                [=]
                {
                    onTextChanged();
                    emit finished(_data);
                });
    }

    void VariableStepWidget::onTextChanged()
    {
        const String s = ((QLineEdit*)sender())->text().toStdString();

        if (sender() == _name.second)
        {
            Su::filterRange(_data.name, s, 'a', 'z');
            setFocus();
        }
        else if (sender() == _min.second)
        {
            String v;
            Su::filterReal(v, s);
            _data.range.x = Char::toFloat(v);
            setFocus();
        }
        else if (sender() == _max.second)
        {
            String v;
            Su::filterReal(v, s);
            _data.range.y = Char::toFloat(v);
            setFocus();
        }
        else if (sender() == _rate.second)
        {
            String v;
            Su::filterReal(v, s);

            _data.rate = Char::toFloat(v);
            setFocus();
        }
        else if (sender() == _value.second)
        {
            String v;
            Su::filterReal(v, s);
            _data.value = Char::toFloat(v);
            setFocus();
        }
    }

    void VariableStepWidget::focusOutEvent(QFocusEvent* event)
    {
        QWidget::focusOutEvent(event);

        if (event->lostFocus())
        {
            if (!anyFocused())
                emit finished(_data);
        }
    }

    void VariableStepWidget::focusInEvent(QFocusEvent* event)
    {
        QWidget::focusInEvent(event);
    }

    void VariableStepWidget::setName(const String& value)
    {
        _name.second->setText(QString::fromStdString(value));
        _data.name = value;
    }

    void VariableStepWidget::setRange(const Vec2F& value)
    {
        _min.second->setText(QString::fromStdString(Char::toString(value.x)));
        _max.second->setText(QString::fromStdString(Char::toString(value.y)));
        _data.range = value;
    }

    void VariableStepWidget::setRate(const R32& value)
    {
        _rate.second->setText(QString::fromStdString(Char::toString(value)));
        _data.rate = value;
    }

    void VariableStepWidget::setValue(const R32& value)
    {
        _value.second->setText(QString::fromStdString(Char::toString(value)));
        _data.value = value;
    }
}  // namespace Jam::Editor
