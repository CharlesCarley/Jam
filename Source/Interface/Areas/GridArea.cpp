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
#include "GridArea.h"
#include "AreaType.h"
#include "GridAreaContent.h"
#include "Interface/Area/Area.h"
#include "Interface/Area/AreaToolbar.h"
#include "Interface/Constants.h"
#include "Interface/Events/LayerEvents.h"
#include "Interface/Extensions.h"
#include "Interface/Widgets/StackedPanel.h"
#include "Interface/Widgets/VerticalScrollArea.h"

namespace Jam::Editor
{

    GridArea::GridArea(AreaCreator* creator,
                       size_t       refId,
                       QWidget*     parent) :
        Area(creator, AtGrid, refId, parent)
    {
        construct();
    }

    GridArea::~GridArea() = default;

    void GridArea::construct()
    {
        Style::apply(this, AreaGridStyle);

        _area = new VerticalScrollArea();
        const auto layout = Style::verticalLayout();
        layout->addWidget(toolbar());
        layout->addWidget(_area, 1);
        setLayout(layout);
    }

    bool GridArea::event(QEvent* event)
    {
        switch ((int)event->type())
        {
        case ProjectOpened:
            _area->setWidget(new GridAreaContent());
            _area->invalidate();
            break;
        default:
            break;
        }
        return Area::event(event);
    }

}  // namespace Jam::Editor
