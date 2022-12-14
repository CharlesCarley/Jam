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
#include "StackedPanelContent.h"
#include "StackedPanel.h"
#include "StackedPanelManager.h"
#include "Interface/Style/Style.h"

namespace Jam::Editor
{

    StackedPanelContent::StackedPanelContent(QWidget* parent) :
        QWidget(parent)
    {
        constructContent();
    }

    void StackedPanelContent::constructContent()
    {
        Style::apply(this, TransparentStyle);
        _manger = new StackedPanelManager();

        // Use addPanel to add items
        // to the manger..
        setLayout(_manger);
    }

    void StackedPanelContent::addPanel(StackedPanel* widget) const
    {
        _manger->addWidget(widget);
    }

    QSize StackedPanelContent::sizeHint() const
    {
        return _manger->totalHeight();
    }

}  // namespace Jam::Editor
