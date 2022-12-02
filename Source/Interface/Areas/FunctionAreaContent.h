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

namespace Jam::Editor
{
    namespace State
    {
        class ExpressionStateObject;
        class VariableStateObject;
    }  // namespace State

    class StringWidget;

    class FunctionAreaContent final : public StackedPanelContent
    {
    public:
        Q_OBJECT
    signals:
        void contentChanged() const;

    private:
        StackedPanel* _panel{nullptr};
        bool          _delayEvent{false};

    public:
        explicit FunctionAreaContent();
        ~FunctionAreaContent() override;

        void addSlider(State::VariableStateObject* obj = nullptr) const;

        void addExpression(State::ExpressionStateObject* obj = nullptr) const;

        void addPoint() const;

    private:
        void construct();

        void loadState();

        // void notifyResize() const;

        void dropWidget(QWidget* widget) const;
    };

}  // namespace Jam::Editor
