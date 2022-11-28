#pragma once
#include "Interface/Widgets/StackedPanelContent.h"
#include "State/FrameStack/FrameStack.h"

namespace Jam::Editor
{
    class I32Widget;

    class GridPropertiesPage final : public StackedPanelContent
    {
        Q_OBJECT
    private:
        I32Widget* _x{nullptr};
        I32Widget* _y{nullptr};

    public:
        explicit GridPropertiesPage(U32 idx);
        ~GridPropertiesPage() override;

    private:
        void construct(U32 idx);
        void connectSignals();
        void disconnectSignals();

        void codeInjected(const FrameStackCode& code, const Vec2F& value) const;

        static void xAxisUpdate(I32 v);

        static void yAxisUpdate(I32 v);
    };

}  // namespace Jam::Editor
