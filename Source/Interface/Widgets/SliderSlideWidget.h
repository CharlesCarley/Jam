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
#include "Math/Vec2F.h"
#include "Math/Vec2I.h"

class QHBoxLayout;
class QLineEdit;

namespace Jam::Editor
{
    class SliderSlideWidget final : public QWidget
    {
        Q_OBJECT
    signals:
        void doubleClicked();
        void valueChanged(R32 val);

    private:
        R32      _value{0};
        R32      _rate{0};
        U8       _captured{0};
        R32      _d{0.f};
        R64      _x{0};
        Vec2F    _range{-R32(0x10000), R32(0x10000)};
        bool     _lock{false};
        String   _label{};
        QString  _display{};
        QPalette _pal{};

    public:
        explicit SliderSlideWidget(QWidget* parent = nullptr);

        void setValue(const R32& val);

        void setValue(const QString& val);

        void setRate(const R32& val);

        void setRange(const Vec2F& val);

        void setLabel(const String& value);

        void setStepData(const VariableStepData& step);

        R32 value() const;

        Vec2F range() const;

        String text() const;

        const String& label() const;

    private:
        void construct();

        void handleSingleTick(const QPointF& d);

        bool isInInnerRect(const QPointF& d) const;

        void mousePressEvent(QMouseEvent* event) override;

        void mouseReleaseEvent(QMouseEvent* event) override;

        void mouseMoveEvent(QMouseEvent* event) override;

        void paintEvent(QPaintEvent* event) override;

        void mouseDoubleClickEvent(QMouseEvent* event) override;

        void updateDisplay();
    };

    inline R32 SliderSlideWidget::value() const
    {
        return _value;
    }

    inline const String& SliderSlideWidget::label() const
    {
        return _label;
    }

}  // namespace Jam::Editor
