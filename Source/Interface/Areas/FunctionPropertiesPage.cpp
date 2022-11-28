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

    FunctionPropertiesPage::~FunctionPropertiesPage()
    {
        disconnectSignals();
    }

    void FunctionPropertiesPage::clear() const
    {
        if (_input0)
            _input0->setString("");
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
            _input0->setString(fn->getText());
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

    void FunctionPropertiesPage::disconnectSignals()
    {
        //
    }

    void FunctionPropertiesPage::onTextChanged(const String& newText)
    {
        if (const auto stack = State::layerStack())
            (void)stack->injectText(newText);
    }

}  // namespace Jam::Editor