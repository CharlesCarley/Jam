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
#include "ExpressionWidget.h"
#include <qboxlayout.h>
#include "IconButton.h"
#include "Interface/Constants.h"
#include "Interface/Extensions.h"
#include "State/App.h"
#include "StringWidget.h"
#include "State/FrameStackManager.h"

namespace Jam::Editor
{

    ExpressionWidget::ExpressionWidget(State::ExpressionStateObject* eso, QWidget* parent) :
        QWidget(parent),
        _state(eso)
    {
        construct();
    }

    void ExpressionWidget::construct()
    {
        View::widgetDefaults(this);

        const auto layout = new QHBoxLayout();
        View::layoutDefaults(layout);

        _line = new StringWidget();
        if (_state)
            _line->setText(_state->text());

        _del = IconButton::create(Icons::Delete);
        View::copyColorRoles(_del, this);

        layout->addWidget(_line, 1);
        layout->addWidget(_del);

        setLayout(layout);
        connectSignals();
    }

    void ExpressionWidget::connectSignals()
    {
        connect(_line,
                &StringWidget::editingFinished,
                this,
                &ExpressionWidget::textEntered);
        connect(_del,
                &QPushButton::clicked,
                this,
                [=]
                { onDelete(); });
    }

    void ExpressionWidget::onDelete()
    {
        State::functionLayer()->removeExpression(_state);
        _state = nullptr;
        emit wantsToDelete();
    }

    void ExpressionWidget::textEntered(const String& text) const
    {
        if (_state)
        {
            _state->setText(text);
            State::layerStack()->notifyStateChange();
        }
    }

    void ExpressionWidget::setText(const String& text) const
    {
        _line->setText(text);
        textEntered(text);
    }

}  // namespace Jam::Editor
