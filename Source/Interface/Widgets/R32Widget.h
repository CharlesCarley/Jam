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
#include "Math/Real.h"

class QHBoxLayout;
class QLineEdit;

namespace Jam::Editor
{
    struct VariableStepData;
    class VariableStepWidget;
    class R32WidgetPrivate;

    class R32Widget final : public QWidget
    {
        Q_OBJECT
    signals:
        void valueChanged(R32 val);
        void stepDataChanged(const VariableStepData& data) const;

    private:
        QHBoxLayout*        _layout{nullptr};
        VariableStepWidget* _step{nullptr};
        R32WidgetPrivate*   _value{nullptr};

    public:
        explicit R32Widget(QWidget* parent = nullptr);
        ~R32Widget() override;

        void setValue(const R32& value) const;

        void setRange(const R32& min, const R32& max) const;

        void setRate(const R32& value) const;

        void setLabel(const String& value) const;

        R32 value() const;

        String string() const;

        String label() const;

        //void setHeight(int hgt) const;

    private:
        void connectSignals();

        void construct();

        void editingFinished() const;

        void doubleClicked() const;

        void returnPressed() const;

        void onStepParamChange(const VariableStepData& data) const;

        void makeEditable(bool edit) const;

        void mouseDoubleClickEvent(QMouseEvent* event) override;

        void keyPressEvent(QKeyEvent* event) override;
    };

}  // namespace Jam::Editor
