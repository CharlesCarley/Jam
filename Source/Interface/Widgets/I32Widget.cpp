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
#include "I32Widget.h"
#include <qtextlayout.h>
#include <QLineEdit>
#include <QWidget>
#include "I32WidgetPrivate.h"
#include "Interface/Areas/OutputArea.h"
#include "Interface/Constants.h"
#include "Interface/Extensions.h"
#include "Utils/Char.h"

namespace Jam::Editor
{

    I32Widget::I32Widget(QWidget* parent) :
        QWidget(parent)
    {
        construct();
    }

    I32Widget::~I32Widget()
    {
        delete _layout;
        delete _i32;
        delete _edit;
    }

    void I32Widget::construct()
    {
        View::applyColorRoles(this);

        _layout = new QHBoxLayout();
        View::layoutDefaults(_layout, Const::AreaPadding);

        _i32  = new I32WidgetPrivate();
        _edit = new QLineEdit();
        _edit->setVisible(false);
        View::applyColorRoles(_edit, QPalette::Base);

        _layout->addWidget(_i32);
        _layout->addWidget(_edit);

        connectEvents();
        setLayout(_layout);
    }

    void I32Widget::editingFinished() const
    {
        makeEditable(false);
    }

    void I32Widget::doubleClicked() const
    {
        makeEditable(true);
    }

    void I32Widget::returnPressed() const
    {
        makeEditable(false);
    }

    void I32Widget::makeEditable(const bool edit) const
    {
        if (!_i32 || !_edit)
            return;

        _i32->setVisible(!edit);
        _edit->setVisible(edit);

        if (edit)
        {
            _edit->setText(_i32->text().c_str());
            _edit->selectAll();
            _edit->setFocus(Qt::MouseFocusReason);
        }
        else
        {
            _i32->setValue(_edit->text());
            _edit->setFocus(Qt::NoFocusReason);
            _i32->setFocus(Qt::MouseFocusReason);
        }
    }

    void I32Widget::connectEvents()
    {
        if (_edit)
        {
            connect(_edit,
                    &QLineEdit::editingFinished,
                    this,
                    &I32Widget::editingFinished);
            connect(_edit,
                    &QLineEdit::returnPressed,
                    this,
                    &I32Widget::returnPressed);
        }
        if (_i32)
        {
            connect(_i32,
                    &I32WidgetPrivate::onDoubleClick,
                    this,
                    &I32Widget::doubleClicked);
            connect(_i32,
                    &I32WidgetPrivate::valueChanged,
                    this,
                    [this](const I32 v)
                    { emit valueChanged(v); });
        }
    }

    void I32Widget::mouseDoubleClickEvent(QMouseEvent* event)
    {
        if (!event)
            return;
        doubleClicked();
        event->accept();
    }

    void I32Widget::keyPressEvent(QKeyEvent* event)
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

    void I32Widget::setValue(const I32& value) const
    {
        if (_i32)
            _i32->setValue(value);
        if (_edit)
            _edit->setText(Char::toString(value).c_str());
    }

    void I32Widget::setRange(const I32& min, const I32& max) const
    {
        if (_i32)
            _i32->setRange({min, max});
    }

    void I32Widget::setLabel(const String& value) const
    {
        if (_i32)
            _i32->setLabel(value);
    }

    I32 I32Widget::value() const
    {
        return _i32 ? _i32->value() : -1;
    }

    String I32Widget::string() const
    {
        return _i32 ? _i32->text() : "";
    }

    String I32Widget::label() const
    {
        if (_i32)
            return _i32->label();
        return {};
    }
}  // namespace Jam::Editor
