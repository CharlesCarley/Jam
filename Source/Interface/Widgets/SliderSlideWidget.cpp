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
#include "SliderSlideWidget.h"
#include <QMouseEvent>
#include <QPainter>
#include "Interface/Extensions.h"
#include "Interface/Style/Palette.h"
#include "Interface/Style/Style.h"
#include "Utils/Char.h"

namespace Jam::Editor
{
    // This is the width of the sub-rect on each side of
    // the whole rectangle, which is used to step +-
    constexpr I32 Sr  = 6;
    constexpr I32 Srh = Sr >> 1;

    SliderSlideWidget::SliderSlideWidget(QWidget* parent) :
        QWidget(parent)
    {
        construct();
    }

    void SliderSlideWidget::construct()
    {
        Style::apply(this, TransparentStyle);
        Palette::getSliderPalette(_pal);

        setMinimumHeight(Style::hint(ButtonHeight));
        setMaximumHeight(Style::hint(ButtonHeight));
    }

    bool SliderSlideWidget::isInInnerRect(const QPointF& d) const
    {
        const QRectF p = geometry();
        return d.x() > p.x() + Sr && d.x() < p.width() - Sr;
    }

    void SliderSlideWidget::setValue(const R32& val)
    {
        if (!equals(val, _value))
        {
            _value = Clamp(val, _range.x, _range.y);

            if (!_lock)
                emit valueChanged(_value);

            updateDisplay();
            update();
        }
    }

    void SliderSlideWidget::setValue(const QString& val)
    {
        const String raw = val.toStdString();
        if (String str; Su::filterReal(str, raw))
            setValue(Char::toFloat(str));
    }

    void SliderSlideWidget::setRate(const R32& val)
    {
        if (!equals(val, _rate))
        {
            _rate = Clamp(val, _range.x, _range.y);
            update();
        }
    }

    void SliderSlideWidget::setRange(const Vec2F& val)
    {
        _range = val;
        setValue(Clamp(_value, _range.x, _range.y));
    }

    void SliderSlideWidget::setLabel(const String& value)
    {
        _label = value;
        updateDisplay();
        update();
    }

    void SliderSlideWidget::setStepData(const VariableStepData& step)
    {
        _lock = true;
        setLabel(step.name);
        setRate(step.rate);
        setRange(step.range);
        setValue(step.value);
        _lock = false;
        setFocus();
    }

    Vec2F SliderSlideWidget::range() const
    {
        return _range;
    }

    String SliderSlideWidget::text() const
    {
        return Char::toString(_value);
    }

    void SliderSlideWidget::handleSingleTick(const QPointF& d)
    {
        if (!isZero(_rate))
        {
            if (d.x() < Sr)
                setValue(_value - _rate);
            else if (d.x() > width() - Sr)
                setValue(_value + _rate);
        }
    }

    void SliderSlideWidget::mousePressEvent(QMouseEvent* event)
    {
        if (!event)
            return;

        if (event->button() == Qt::LeftButton)
        {
            if (const QPointF d = event->position();
                isInInnerRect(d))
                _captured = true;
            else
                handleSingleTick(d);
        }
        else
            QWidget::mousePressEvent(event);
    }

    void SliderSlideWidget::mouseDoubleClickEvent(QMouseEvent* event)
    {
        if (!event)
            return;

        _captured = false;

        if (const QPointF d = event->position(); isInInnerRect(d))
            emit doubleClicked();
        else
            handleSingleTick(d);
    }

    void SliderSlideWidget::updateDisplay()
    {
        // needs to be manually updated any
        // time one of the values change
        _display = QString::fromStdString(
            StringUtils::join(
                _label,
                ":=",
                Char::toString(_value)));
    }

    void SliderSlideWidget::mouseReleaseEvent(QMouseEvent* event)
    {
        if (!event)
            return;

        if (event->button() == Qt::LeftButton)
        {
            _captured = false;
            event->accept();
        }
        else
            QWidget::mouseReleaseEvent(event);
    }

    void SliderSlideWidget::mouseMoveEvent(QMouseEvent* event)
    {
        if (!event)
            return;

        if (_captured && !isZero(_rate))
        {
            const R32 w  = R32(width());
            const R32 u = Clamp(R32(event->position().x()), 0.f - _rate, w + _rate) / w;
            _d = _range.dmm() * u + _range.x;
            _d -= fmod(_d, _rate);
            setValue(_d);
        }
        else
            QWidget::mouseMoveEvent(event);
    }

    void SliderSlideWidget::paintEvent(QPaintEvent* event)
    {
        QPainter paint(this);

        // base fill
        paint.fillRect(geometry(), _pal.shadow());
        paint.setPen(_pal.shadow().color().lighter(175));
        QRect contracted = geometry().adjusted(0, 0, -1, -1);
        paint.drawRect(contracted);

        // fill the value of slider
        contracted.adjust(2, 2, -1, -1);
        const R32 ws = (fabs(_range.x) + R32(_value)) / _range.dmm() * R32(contracted.width());
        paint.fillRect(contracted.x(), contracted.y(), I32(ws), contracted.height(), _pal.mid());

        paint.setPen(_pal.text().color());
        paint.drawText(contracted.width() >> 1, paint.fontMetrics().height(), _display);

        const I32 my = contracted.y() + (contracted.height() >> 1);
        const I32 mx = contracted.right() - Srh;

        const QLine lines[] = {
            QLine(1 + contracted.left(), my, 1 + Sr, my),
            QLine(contracted.right() - Sr, my, contracted.right(), my),
            QLine(mx, my - Srh, mx, my + Srh),
        };
        paint.drawLines(lines, 3);
    }

}  // namespace Jam::Editor
