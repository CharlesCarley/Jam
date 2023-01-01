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
#include "SliderEditWidget.h"
#include <qevent.h>
#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>
#include "Interface/Areas/OutputArea.h"
#include "Interface/Style/Style.h"
#include "Utils/Char.h"

namespace Jam::Editor
{
    SliderEditWidget::SliderEditWidget(QWidget* parent) :
        QWidget(parent)
    {
        construct();
    }

    void SliderEditWidget::construct()
    {
        const auto layout = Style::horizontalLayout(2, 1);
        makePair(_name, "id");
        makePair(_value, ":=");
        makePair(_min, ">=");
        makePair(_max, "<=");
        makePair(_rate, "+=");

        layout->addWidget(_name.first, 0, Qt::AlignRight);
        layout->addWidget(_name.second, 1);

        layout->addWidget(_value.first, 0, Qt::AlignRight);
        layout->addWidget(_value.second, 1);

        layout->addWidget(_min.first, 0, Qt::AlignRight);
        layout->addWidget(_min.second, 1);

        layout->addWidget(_max.first, 0, Qt::AlignRight);
        layout->addWidget(_max.second, 1);

        layout->addWidget(_rate.first, 0, Qt::AlignRight);
        layout->addWidget(_rate.second, 1);

        setLayout(layout);
    }

    bool SliderEditWidget::anyFocused() const
    {
        return _rate.second->hasFocus() ||
               _name.second->hasFocus() ||
               _min.second->hasFocus() ||
               _max.second->hasFocus() ||
               _value.second->hasFocus();
    }

    void SliderEditWidget::exitEditTest()
    {
        // needs to be bound to the R32Widget
        if (!anyFocused())
            emit stepParamChange(_data);
    }

    void SliderEditWidget::makePair(LabelLineEditPair& dest,
                                    const char*        name)
    {
        dest.first = Style::text(name);
        dest.first->setAlignment(Qt::AlignCenter);
        dest.first->setFocusPolicy(Qt::NoFocus);

        dest.second = Style::darkLine();
        dest.second->setFocusPolicy(Qt::StrongFocus);

        connect(dest.second,
                &QLineEdit::editingFinished,
                this,
                &SliderEditWidget::onTextChanged);

        connect(dest.second,
                &QLineEdit::returnPressed,
                this,
                [=]
                {
                    onTextChanged();
                    emit stepParamChange(_data);
                    setFocus();
                });
    }

    void SliderEditWidget::setStepData(const VariableStepData& data)
    {
        _data = data;
        _lock = true;
        _name.second->setText(QString::fromStdString(_data.name));
        _min.second->setText(QString::number((qreal)_data.range.x));
        _max.second->setText(QString::number((qreal)_data.range.y));
        _rate.second->setText(QString::number((qreal)_data.rate));
        _value.second->setText(QString::number((qreal)_data.value));
        _lock = false;
    }

    void SliderEditWidget::onTextChanged()
    {
        if (_lock)
            return;

        const QLineEdit *edit = (QLineEdit*)sender();
        if (!edit)
            return;

        const String source = edit->text().toStdString();

        // name, value, min, max, rate
        if (sender() == _name.second)
            Su::filterAZaz(_data.name, source, 1);
        else if (sender() == _value.second)
        {
            String v;
            Su::filterReal(v, source);
            _data.value = Char::toFloat(v);
        }
        else if (sender() == _min.second)
        {
            String v;
            Su::filterReal(v, source);
            _data.range.x = Char::toFloat(v);
        }
        else if (sender() == _max.second)
        {
            String v;
            Su::filterReal(v, source);
            _data.range.y = Char::toFloat(v);
        }
        else if (sender() == _rate.second)
        {
            String v;
            Su::filterReal(v, source);
            _data.rate = Char::toFloat(v);
        }

        exitEditTest();
    }

    void SliderEditWidget::focusOutEvent(QFocusEvent* event)
    {
        if (event && event->lostFocus())
            exitEditTest();
    }

    void SliderEditWidget::focusInEvent(QFocusEvent* event)
    {
        Q_ASSERT(_value.second);
        // focus the most likely element to change.
        if (_value.second)
            _value.second->setFocus();
    }
}  // namespace Jam::Editor
