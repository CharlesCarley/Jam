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
#include "Math/Vec2F.h"

class QLabel;
class QLineEdit;

namespace Jam::Editor
{
    struct VariableStepData
    {
        String name{};
        Vec2F  range{0.f, 0};
        R32    rate{0};
    };

    class VariableStepWidget final : public QWidget
    {
        Q_OBJECT
    signals:
        void finished(const VariableStepData& data);

    private:
        QLineEdit* _name{nullptr};
        QLineEdit* _min{nullptr};
        QLineEdit* _max{nullptr};
        QLineEdit* _rate{nullptr};

        VariableStepData _data;

    public:
        explicit VariableStepWidget(QWidget* parent = nullptr);

        void setName(const String& value);

        void setRange(const Vec2F& value);

        void setRate(const R32& value);

    private:
        void construct();

        void connectSignals();

        void onTextChanged();

        void focusOutEvent(QFocusEvent* event) override;

        void focusInEvent(QFocusEvent* event) override;
    };

}  // namespace Jam::Editor
