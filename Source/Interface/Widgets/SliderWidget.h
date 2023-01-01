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
#include <QWidget>
#include "Interface/Widgets/SliderEditWidget.h"
#include "Math/Real.h"

class QHBoxLayout;
class QLineEdit;

namespace Jam::Editor
{
    struct VariableStepData;
    class SliderEditWidget;
    class SliderSlideWidget;

    class SliderWidget final : public QWidget
    {
        Q_OBJECT
    signals:
        void stepDataChanged(const VariableStepData& data) const;

    private:
        QHBoxLayout*       _layout{nullptr};
        SliderEditWidget*  _step{nullptr};
        SliderSlideWidget* _value{nullptr};
        // Caches the get/sets on the step data to have faster access to
        // the contents of the step data via stepDataChanged signals
        VariableStepData _data{};

    public:
        explicit SliderWidget(QWidget* parent = nullptr);
        ~SliderWidget() override;

        void setValue(const R32& value);

        void setRange(const R32& min, const R32& max);

        void setRate(const R32& rate);

        void setLabel(const String& label);

        R32 value() const;

        Vec2F range() const;

        String string() const;

        String label() const;

    private:
        void connectSignals();

        void construct();

        void editingFinished() const;

        void returnPressed() const;

        void onDoubleClicked() const;

        void onStepParamChange(const VariableStepData& data);

        void onValueChanged(const R32& value);

        void makeEditable(bool edit) const;

        void mouseDoubleClickEvent(QMouseEvent* event) override;

        void keyPressEvent(QKeyEvent* event) override;
    };

}  // namespace Jam::Editor
