#pragma once
#include "Interface/Widgets/StackedPanelContent.h"
#include "State/FrameStack/FrameStack.h"

namespace Jam::Editor
{
    class StringWidget;

    class FunctionPropertiesPage final : public StackedPanelContent
    {
        Q_OBJECT
    private:
        StringWidget* _input0{nullptr};

    public:
        explicit FunctionPropertiesPage(U32 idx);
        ~FunctionPropertiesPage() override;

        void clear() const;
    private:
        void construct(U32 idx);
        void connectSignals();
        void disconnectSignals();

        static void onTextChanged(const String& newText);
    };

}  // namespace Jam::Editor
