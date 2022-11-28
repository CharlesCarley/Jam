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
#include <QWidget>
#include "Interface/Extensions.h"
#include "Interface/Widgets/StackedPanel.h"
#include "Interface/Widgets/StringWidget.h"
#include "State/App.h"
#include "State/FrameStack/FunctionLayer.h"
#include "State/FrameStackManager.h"

namespace Jam::Editor
{
    FunctionPropertiesPage::FunctionPropertiesPage(const U32 idx)
    {
        construct(idx);
    }

    FunctionPropertiesPage::~FunctionPropertiesPage() = default;

    void FunctionPropertiesPage::clear() const
    {
        if (_input0)
            _input0->setText("");
    }

    void FunctionPropertiesPage::construct(U32 idx)
    {
        View::applyColorRoles(this);

        const auto panel = new StackedPanel();
        panel->setLabel("Functions");

        _input0 = new StringWidget();

        if (const auto state = State::layerStack())
        {
            if (const auto fn = state->cast<Layers::FunctionLayer,
                                            FunctionType>(idx))
                _input0->setText(fn->getText());
        }

        panel->addWidget(_input0);

        connectSignals();

        addPanel(panel);
    }

    void FunctionPropertiesPage::connectSignals()
    {
        connect(_input0,
                &StringWidget::editingFinished,
                this,
                &FunctionPropertiesPage::onTextChanged);
        connect(_input0,
                &StringWidget::returnPressed,
                this,
                &FunctionPropertiesPage::onTextChanged);
    }

    void FunctionPropertiesPage::onTextChanged(const String& newText)
    {
        if (const auto stack = State::layerStack())
            (void)stack->injectText(newText);
    }

}  // namespace Jam::Editor