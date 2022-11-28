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

class QHBoxLayout;
class QLineEdit;
class QSpinBox;

namespace Jam::Editor
{
    class I32WidgetPrivate;

    class I32Widget final : public QWidget
    {
        Q_OBJECT
    signals:
        void valueChanged(I32 val);

    private:
        QHBoxLayout*      _layout{nullptr};
        QLineEdit*        _edit{nullptr};
        I32WidgetPrivate* _i32{nullptr};

    public:
        explicit I32Widget(QWidget* parent = nullptr);
        ~I32Widget() override;

        void setValue(const I32& value) const;

        void setRange(const I32& min, const I32& max) const;

        void setLabel(const String& value) const;

        I32 value() const;

        String string() const;

        String label() const;

    private:
        void connectEvents();
        void construct();

        void editingFinished() const;
        void doubleClicked() const;
        void returnPressed() const;

        void makeEditable(bool edit) const;

        void mouseDoubleClickEvent(QMouseEvent* event) override;
        void keyPressEvent(QKeyEvent* event) override;
    };

}  // namespace Jam::Editor
