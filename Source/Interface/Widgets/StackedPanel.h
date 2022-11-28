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
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include "Math/Integer.h"

namespace Jam::Editor
{
    constexpr I32 StackedPanelMargin         = 8;
    constexpr I32 StackedPanelSpacing        = 0;
    constexpr I32 StackedPanelContentMargin  = 4;
    constexpr I32 StackedPanelContentSpacing = 2;

    class StackedPanel final : public QWidget
    {
        Q_OBJECT
    private:
        QLabel*      _title{nullptr};
        QVBoxLayout* _layout{nullptr};
        QVBoxLayout* _contentLayout{nullptr};

    public:
        explicit StackedPanel(QWidget* parent = nullptr);
        ~StackedPanel() override;

        void addWidget(QWidget* widget, int expand = 1) const;
        void addLayout(QLayout* widget, int expand = 1) const;

        void setLabel(const QString& label) const;

        QSize sizeHint() const override;

    private:
        void construct();
    };

}  // namespace Jam::Editor
