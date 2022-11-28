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
#include "Interface/Area/AreaEdgeRect.h"
#include <QBoxLayout>
#include "AreaBranch.h"
#include "AreaNode.h"
#include "Interface/Area/Area.h"
#include "Interface/Area/AreaEdge.h"
#include "Interface/Extensions.h"

namespace Jam::Editor
{
    AreaEdgeRect::AreaEdgeRect(Area* root) :
        QWidget()
    {
        construct(root);
    }

    AreaEdgeRect::~AreaEdgeRect()
    {
        delete _top;
        delete _left;
        delete _right;
        delete _bottom;
    }

    void AreaEdgeRect::construct(Area* root)
    {
        View::widgetDefaults(this);
        View::emptyWidget(this);

        _vLayout = new QVBoxLayout();
        View::layoutDefaults(_vLayout);

        _hLayout = new QHBoxLayout();
        View::layoutDefaults(_hLayout);

        if (_mask & AreaEdgeLeft)
        {
            _left = new AreaEdge(Qt::Vertical, this);
            bindContextMenu(_left);
            _hLayout->addWidget(_left, 0, Qt::AlignLeft);
        }

        _hLayout->addWidget(root, 1);

        if (_mask & AreaEdgeRight)
        {
            _right = new AreaEdge(Qt::Vertical, this);
            bindContextMenu(_right);
            _hLayout->addWidget(_right, 0, Qt::AlignRight);
        }

        if (_mask & AreaEdgeTop)
        {
            _top = new AreaEdge(Qt::Horizontal, this);
            bindContextMenu(_top);
            _vLayout->addWidget(_top, 0);
        }

        _vLayout->addLayout(_hLayout, 1);

        if (_mask & AreaEdgeBottom)
        {
            _bottom = new AreaEdge(Qt::Horizontal, this);
            bindContextMenu(_bottom);
            _vLayout->addWidget(_bottom, 0);
        }

        setLayout(_vLayout);
    }
    QVBoxLayout* AreaEdgeRect::container() const
    {
        return _vLayout;
    }

    void AreaEdgeRect::updateMask(const int mask)
    {
        _mask = mask;

        applyEdgeVisibility(_top, (_mask & AreaEdgeTop) != 0);
        applyEdgeVisibility(_bottom, (_mask & AreaEdgeBottom) != 0);
        applyEdgeVisibility(_left, (_mask & AreaEdgeLeft) != 0);
        applyEdgeVisibility(_right, (_mask & AreaEdgeRight) != 0);
    }

    void AreaEdgeRect::applyEdgeVisibility(AreaEdge* edge, bool show)
    {
        if (edge)
            edge->setVisible(show);
    }

    void AreaEdgeRect::contextMenu(Qt::Orientation ori, const QPoint& at)
    {
        emit wantsContextMenu(ori, ori == Qt::Horizontal ? Qt::Vertical : Qt::Horizontal, at);
    }

    void AreaEdgeRect::bindContextMenu(const AreaEdge* splitter)
    {
        connect(splitter,
                &AreaEdge::wantsContextMenu,
                this,
                &AreaEdgeRect::contextMenu);
    }

    int AreaEdgeRect::computeMask(const AreaNode* branch,
                                  const AreaNode* current,
                                  int             mask,
                                  const int       values[4])
    {
        if (!branch)  // unlikely
            return mask;
        // can current be null?

        if (const AreaBranch* areaBranch = (AreaBranch*)branch->content())
        {
            if (const int orientation = areaBranch->orientation();
                orientation == Qt::Horizontal)
            {
                if (branch->left() == current)
                    mask &= ~values[0];
                if (branch->right() == current)
                    mask &= ~values[1];
            }
            else
            {
                if (branch->left() == current)
                    mask &= ~values[2];
                if (branch->right() == current)
                    mask &= ~values[3];
            }
            // else let the mask fall through unmodified
        }
        return mask;
    }

    int AreaEdgeRect::computeMask(const AreaNode*    parent,
                                  const AreaContent* current,
                                  int                mask,
                                  const int          values[4])
    {
        if (!parent)
            return mask;

        if (const AreaBranch* branch = parent->branch())
        {
            const int ori = branch->orientation();

            if (branch->leftContent() == current)
            {
                if (ori == Qt::Horizontal)
                    mask &= ~values[0];
                else
                    mask &= ~values[1];
            }
            else if (branch->rightContent() == current)
            {
                if (ori == Qt::Horizontal)
                    mask &= ~values[2];
                else
                    mask &= ~values[3];
            }
        }
        return mask;
    }

    int AreaEdgeRect::computeMask(const AreaNode* parent,
                                  const AreaNode* current,
                                  const int       type,
                                  const int       ori)
    {
        int mask;
        if (ori == Qt::Horizontal)  // is cross
        {
            if (type == 0)
                mask = AreaEdgeVLeft;
            else
                mask = AreaEdgeVRight;
        }
        else if (ori == Qt::Vertical)
        {
            if (type == 0)
                mask = AreaEdgeHTop;
            else
                mask = AreaEdgeHBottom;
        }
        else
            mask = AreaEdgeAll;

        if (!parent)
            return mask;

        if (const AreaContent* content = parent->content())
        {
            if (const AreaNode* areaNode = content->parentNode();
                areaNode && areaNode->isBranch())
            {
                constexpr int values[4] = {
                    AreaEdgeRight,
                    AreaEdgeLeft,
                    AreaEdgeLeft,
                    AreaEdgeLeft,
                };
                mask = computeMask(areaNode, current, mask, values);
            }
        }
        return mask;
    }
}  // namespace Jam::Editor
