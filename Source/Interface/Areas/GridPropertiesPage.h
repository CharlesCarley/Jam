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
