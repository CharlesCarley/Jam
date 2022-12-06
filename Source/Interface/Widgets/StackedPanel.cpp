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
#include "StackedPanel.h"
#include "Interface/Extensions.h"
#include "Interface/Style/Style.h"

namespace Jam::Editor
{
    StackedPanel::StackedPanel(QWidget* parent) :
        QWidget(parent)
    {
        construct();
    }

    StackedPanel::~StackedPanel() = default;

    void StackedPanel::addWidget(
        QWidget*  widget,
        const int expand) const
    {
        _layout->addWidget(widget, expand);
    }

    void StackedPanel::remove(QWidget* widget) const
    {
        _layout->removeWidget(widget);
    }

    void StackedPanel::addLayout(QLayout* widget, const int expand) const
    {
        _layout->addLayout(widget, expand);
    }

    QSize StackedPanel::sizeHint() const
    {
        return View::calcMaxHeight(_layout);
    }

    void StackedPanel::construct()
    {
        Style::apply(this, TransparentStyle);
        _layout = Style::verticalLayout(
            StackedPanelContentMargin,
            StackedPanelContentSpacing);

        setLayout(_layout);
    }

}  // namespace Jam::Editor
