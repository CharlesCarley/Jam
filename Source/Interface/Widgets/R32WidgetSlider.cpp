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
#include "R32WidgetSlider.h"
#include <QMouseEvent>
#include <QPainter>
#include "Interface/Constants.h"
#include "Interface/Extensions.h"
#include "Interface/Style/Palette.h"
#include "Utils/Char.h"

namespace Jam::Editor
{
    constexpr I32 Sr = 6;

    constexpr QLineF MidLines[] = {
        {     0.0, 0.5 * Sr, 1.0 * Sr, 0.5 * Sr}, // x
        {0.5 * Sr,      0.0, 0.5 * Sr, 1.0 * Sr}, // y
    };
    constexpr QLineF Hl = MidLines[0];
    constexpr QLineF Vl = MidLines[1];

    R32WidgetSlider::R32WidgetSlider(QWidget* parent) :
        QWidget(parent)
    {
        construct();
    }

    void R32WidgetSlider::construct()
    {
        Style::apply(this, TransparentStyle);
        Palette::getSliderPalette(_pal);

        setMinimumHeight(Style::hint(ButtonHeight));
        setMaximumHeight(Style::hint(ButtonHeight));
    }

    bool R32WidgetSlider::isInInnerRect(const QPointF& d) const
    {
        const QRectF p = geometry();
        return d.x() > p.x() + Sr && d.x() < p.width() - Sr;
    }

    void R32WidgetSlider::setValue(const R32& val)
    {
        if (!equals(val, _value))
        {
            _value = Clamp(val, _range.x, _range.y);
            if (!_lock)
                emit valueChanged(_value);

            update();
        }
    }

    void R32WidgetSlider::setValue(const QString& val)
    {
        String       str;
        const String raw = val.toStdString();
        if (Su::filterReal(str, raw))
            setValue(Char::toFloat(str));
    }

    void R32WidgetSlider::setRate(const R32& val)
    {
        if (!equals(val, _rate))
        {
            _rate = Clamp(val, _range.x, _range.y);
            update();
        }
    }

    void R32WidgetSlider::setRange(const Vec2F& val)
    {
        _range = val;
        setValue(Clamp(_value, _range.x, _range.y));
    }

    void R32WidgetSlider::setLabel(const String& value)
    {
        _label = Su::join(value, " := ");
        update();
    }

    void R32WidgetSlider::setStepData(const VariableStepData& step)
    {
        _lock = true;
        setLabel(step.name);
        setRate(step.rate);
        setRange(step.range);
        setValue(step.value);
        _lock = false;
        setFocus();
    }

    String R32WidgetSlider::text() const
    {
        return Char::toString(_value);
    }

    void R32WidgetSlider::handleSingleTick(const QPointF& d)
    {
        if (d.x() < Sr)
            setValue(_value - _rate);
        else if (d.x() > width() - Sr)
            setValue(_value + _rate);
    }

    void R32WidgetSlider::mousePressEvent(QMouseEvent* event)
    {
        if (!event)
            return;

        if (event->button() == Qt::LeftButton)
        {
            if (const QPointF d = event->position();
                isInInnerRect(d))
                _cap = true;
            else
                handleSingleTick(d);
        }
        else
            QWidget::mousePressEvent(event);
    }

    void R32WidgetSlider::mouseDoubleClickEvent(QMouseEvent* event)
    {
        if (!event)
            return;

        _cap = false;
        event->accept();

        if (const QPointF d = event->position();
            isInInnerRect(d))
            emit doubleClicked();
        else
            handleSingleTick(d);
    }

    void R32WidgetSlider::mouseReleaseEvent(QMouseEvent* event)
    {
        if (!event)
            return;

        if (event->button() == Qt::LeftButton)
        {
            _cap = false;
            event->accept();
        }
        else
            QWidget::mouseReleaseEvent(event);
    }

    void R32WidgetSlider::mouseMoveEvent(QMouseEvent* event)
    {
        if (!event)
            return;

        if (_cap)
        {
            QPointF p1 = event->position();
            if (p1.x() > width())
                p1.setX(width());
            if (p1.x() < 0)
                p1.setX(0);

            p1 /= width();
            _d = {R32(p1.x()), R32(p1.x())};
            _d.clamp(0, 1);
            setValue(_range.dmm() * _d.x + _range.x);
        }
        else
            QWidget::mouseMoveEvent(event);
    }

    void R32WidgetSlider::paintEvent(QPaintEvent* event)
    {
        QPainter           paint(this);
        const QFontMetrics metrics = paint.fontMetrics();

        const I32 w = width(), h = height();
        const I32 th = metrics.height();
        const I32 r0 = w - Sr;
        const I32 r1 = (h >> 1) - (Sr >> 1);

        paint.fillRect(0, 0, w, h, _pal.shadow());
        paint.setPen(_pal.window().color());
        paint.drawRect(1, 1, w - 1, h - 1);

        const R32 ws = (fabs(_range.x) + R32(_value)) / _range.dmm() * R32(w);
        paint.fillRect(0, 0, I32(ws), h, _pal.mid());
        paint.setPen(_pal.text().color());

        paint.drawText(
            w >> 1,
            th,
            StringUtils::join(
                _label,
                Char::toString(_value))
                .c_str());

        paint.setPen(_pal.brightText().color());
        paint.drawLine(
            I32(Hl.x1()),
            r1 + I32(Hl.y1()),
            I32(Hl.x2()),
            r1 + I32(Hl.y2()));

        paint.drawLine(
            r0 + I32(Hl.x1()),
            r1 + I32(Hl.y1()),
            r0 + I32(Hl.x2()),
            r1 + I32(Hl.y2()));

        paint.drawLine(
            r0 + I32(Vl.x1()),
            r1 + I32(Vl.y1()),
            r0 + I32(Vl.x2()),
            r1 + I32(Vl.y2()));
    }

}  // namespace Jam::Editor
