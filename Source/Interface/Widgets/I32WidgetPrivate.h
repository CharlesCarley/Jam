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
#include "Math/Integer.h"
#include "Math/Vec2F.h"
#include "Math/Vec2I.h"

class QHBoxLayout;
class QLineEdit;
class QSpinBox;

namespace Jam::Editor
{
    class I32WidgetPrivate final : public QWidget
    {
        Q_OBJECT
    signals:
        void onDoubleClick();
        void valueChanged(I32 val);

    private:
        I32      _val{0};
        QPalette _pal;
        U8       _cap{0};
        Vec2F    _d{0.f, 0};
        Vec2I    _mm{-I32(0x10000), I32(0x10000)};
        String   _label;

    public:
        explicit I32WidgetPrivate(QWidget* parent = nullptr);

        void setValue(const I32& val);

        void setValue(const QString& val);

        void setRange(const Vec2I& val);

        void setLabel(const String& value);

        I32 value() const;

        String text() const;

        String label() const;

    protected:
        void construct();

        void handleSingleTick(const QPointF& d);
        bool isInInnerRect(const QPointF& d) const;

        void mousePressEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void paintEvent(QPaintEvent* event) override;
        void mouseDoubleClickEvent(QMouseEvent* event) override;
    };

    inline I32 I32WidgetPrivate::value() const
    {
        return _val;
    }

}  // namespace Jam::Editor
