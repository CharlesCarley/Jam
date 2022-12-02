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
#include "VariableWidget.h"
#include <qboxlayout.h>
#include "IconButton.h"
#include "Interface/Extensions.h"
#include "R32Widget.h"
#include "State/App.h"
#include "State/FrameStackManager.h"
#include "VariableStepWidget.h"

namespace Jam::Editor
{

    VariableWidget::VariableWidget(State::VariableStateObject* obj, QWidget* parent) :
        QWidget(parent),
        _state{obj}
    {
        construct();
    }

    void VariableWidget::construct()
    {
        Q_ASSERT(_state);
        View::widgetDefaults(this);
        View::buttonDefaults(this);

        const auto layout = new QHBoxLayout();
        View::layoutDefaults(layout);

        _line = new R32Widget();
        _line->setRange(_state->range().x, _state->range().y);
        _line->setRate(_state->rate());
        _line->setValue(_state->value());
        _line->setLabel(_state->name());

        _del = IconButton::create(Icons::Delete);
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
                { onDelete(); });
        connect(_line,
                &R32Widget::stepDataChanged,
                this,
                &VariableWidget::stepDataChanged);
        connect(_line,
                &R32Widget::valueChanged,
                this,
                &VariableWidget::onValueChange);
    }

    void VariableWidget::stepDataChanged(const VariableStepData& data) const
    {
        if (_state)
        {
            _state->setRange(data.range);
            _state->setName(data.name);
            _state->setRate(data.rate);
            _state->setValue(data.value);
        }
    }

    void VariableWidget::onValueChange(const R32& data) const
    {
        _state->setValue(data);

        State::layerStack()->notifyStateChange(this);
    }

    void VariableWidget::onDelete()
    {
        State::functionLayer()->removeVariable(_state);
        _state = nullptr;
        State::layerStack()->notifyStateChange(this);
        emit wantsToDelete();
    }

    void VariableWidget::setName(const String& name) const
    {
        _line->setLabel(name);
    }

    void VariableWidget::setRange(const Vec2F& range) const
    {
        _line->setRange(range.x, range.y);
    }

}  // namespace Jam::Editor
