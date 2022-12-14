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
#include "SliderEditWidget.h"
#include "Utils/String.h"

class QPushButton;

namespace Jam::Editor
{
    struct VariableStepData;

    namespace State
    {
        class VariableStateObject;
    }

    class SliderEditWidget;
    class SliderWidget;

    class VariableWidget final : public QWidget
    {
        Q_OBJECT
    signals:
        void deleteVariable(size_t refid);
        void variableChanged(size_t refid, const VariableStepData& data);
        
    private:
        SliderWidget*     _line{nullptr};
        QPushButton*      _del{nullptr};
        SliderEditWidget* _edit{nullptr};
        size_t            _refId{JtNpos};
        VariableStepData  _stepData;

    public:
        explicit VariableWidget(QWidget* parent = nullptr);

        void setName(const String& name);

        void setRange(const Vec2F& range);

        void setRate(const R32& rate);

        void setValue(const R32& value);

        void setRefId(size_t id);

        size_t refId() const;

        const VariableStepData& data();

    private:
        void construct();

        void connectSignals();

        void onStepDataChanged(const VariableStepData& data);
    };

    inline void VariableWidget::setRefId(const size_t id)
    {
        _refId = id;
    }

    inline size_t VariableWidget::refId() const
    {
        return _refId;
    }

    inline const VariableStepData& VariableWidget::data()
    {
        return _stepData;
    }

}  // namespace Jam::Editor
