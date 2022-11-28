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
#include "Interface/Area/AreaLeaf.h"
#include <QBoxLayout>
#include <QCoreApplication>
#include <QMenu>
#include "Interface/Area/Area.h"
#include "Interface/Area/AreaBranch.h"
#include "Interface/Area/AreaNode.h"
#include "Interface/Events/BranchEvent.h"
#include "Interface/Events/SwitchEvent.h"

namespace Jam::Editor
{

    AreaLeaf::AreaLeaf(Area* root, const int mask) :
        AreaContent(false),
        _leafInnerArea(root),
        _mask(mask)
    {
        construct();
    }

    AreaLeaf::~AreaLeaf()
    {
        delete _edges;
    }

    Area* AreaLeaf::contents() const
    {
        return _leafInnerArea;
    }

    int32_t AreaLeaf::type() const
    {
        if (_leafInnerArea)
            return _leafInnerArea->type();
        return -1;
    }

    void AreaLeaf::updateMask()
    {
        if (_edges)
            _edges->updateMask(computeMask());
    }

    bool AreaLeaf::propagate(QEvent* event)
    {
        if (!event)
            return false;

        if (_leafInnerArea)
            return _leafInnerArea->propagate(event);

        return AreaContent::propagate(event);
    }

    void AreaLeaf::construct()
    {
        _edges = new AreaEdgeRect(_leafInnerArea);
        connect(_edges, &AreaEdgeRect::wantsContextMenu, this, &AreaLeaf::contextMenu);

        connect(_leafInnerArea, &Area::wantsContextSwitch, this, [=](int at)
                { QCoreApplication::postEvent(parentNode(), new SwitchEvent(at)); });

        setLayout(_edges->container());
    }

    void AreaLeaf::contextMenu(const Qt::Orientation from,
                               const Qt::Orientation to,
                               const QPoint&         at)
    {
        QMenu splitMenu(this);
        splitMenu.addAction("Split", [=]
                            { QCoreApplication::postEvent(
                                  parentNode(),
                                  new BranchEvent(from, to)); });
        splitMenu.exec(at);
    }

    int AreaLeaf::computeMask()
    {
        if (!_parent)
            return _mask;


        constexpr int values[4] = {
            AreaEdgeRight,
            AreaEdgeBottom,
            AreaEdgeLeft,
            AreaEdgeTop,
        };

        _mask = AreaEdgeAll;

        if (const AreaNode* parent = _parent->parentNode();
            parent && parent->isBranch())
        {
            _mask = AreaEdgeRect::computeMask(parent, this, _mask, values);

            while (parent)
            {
                const auto content = parent->content();
                if (parent = parent->parentNode(); parent)
                    _mask = AreaEdgeRect::computeMask(parent, content, _mask, values);
            }
        }
        return _mask;
    }
    
}  // namespace Jam::Editor
