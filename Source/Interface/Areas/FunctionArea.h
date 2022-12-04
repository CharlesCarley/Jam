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
#include "Interface/Area/Area.h"
#include "State/FrameStack/FunctionLayer.h"

namespace Jam::Editor
{
    struct VariableStepData;
    class VariableWidget;
    class ExpressionWidget;
    class VerticalScrollArea;
    class FunctionAreaContent;

    using ExpressionMap = HashTable<size_t, ExpressionWidget*>;
    using VariableMap   = HashTable<size_t, VariableWidget*>;

    class FunctionArea final : public Area
    {
        Q_OBJECT
    private:
        VerticalScrollArea*   _area{nullptr};
        FunctionAreaContent*  _func{nullptr};
        State::FunctionLayer* _function{nullptr};

        ExpressionMap _expressions;
        VariableMap   _variables;

    public:
        explicit FunctionArea(AreaCreator* creator,
                              size_t       refId  = JtNpos,
                              QWidget*     parent = nullptr);
        ~FunctionArea() override;

        VariableWidget* addSlider();

        ExpressionWidget* addExpression();

        void addPoint() const;

    private:
        void construct();

        void loadStack();

        void constructTools(const AreaToolBar* tools);

        void constructContent();

        void displayOptions(QWidget* widget);

        void onDeleteVariable(size_t refid, const VariableWidget* widget);

        void onDeleteExpression(size_t refid, const ExpressionWidget* widget);

        void onVariableChanged(size_t refid, const VariableStepData& data) const;

        void onExpressionChanged(size_t refid, const String& text) const;

        void onContentChanged() const;
    };
}  // namespace Jam::Editor
