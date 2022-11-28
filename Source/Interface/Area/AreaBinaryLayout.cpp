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
#include "Interface/Area/AreaBinaryLayout.h"
#include <QApplication>
#include <QContextMenuEvent>
#include <QCoreApplication>
#include <QPainter>
#include "Math/Real.h"
#include "Interface/Area/AreaBinaryLayoutPrivate.h"
#include "Interface/Area/AreaNode.h"
#include "Interface/Constants.h"
#include "Interface/Extensions.h"

namespace Jam::Editor
{
    void showLayoutItem(const QLayoutItem* item, const bool show)
    {
        if (QWidget* widget = item->widget())
        {
            if (widget->isVisible() != show)
                widget->setVisible(show);
        }
    }

    AreaBinaryLayout::AreaBinaryLayout(const Qt::Orientation& orientation,
                                       AreaNode*              left,
                                       AreaNode*              right) :
        QLayout(),
        _orientation(orientation)
    {
        construct(left, right);
    }

    AreaBinaryLayout::~AreaBinaryLayout()
    {
        while (const QLayoutItem* item = takeAt(0))
        {
            delete item->widget();
            delete item;
        }
        _items.clear();
    }

    const Qt::Orientation& AreaBinaryLayout::orientation() const
    {
        return _orientation;
    }

    QSize AreaBinaryLayout::sizeHint() const
    {
        if (const QWidget* widget = parentWidget())
            return widget->size();
        return {};
    }

    void AreaBinaryLayout::addItem(QLayoutItem* item)
    {
        if (item != nullptr)
            _items.append(item);
    }

    QLayoutItem* AreaBinaryLayout::itemAt(const int index) const
    {
        if (index >= 0 && index < _items.size())
            return _items.at(index);
        return nullptr;
    }

    QLayoutItem* AreaBinaryLayout::takeAt(const int index)
    {
        if (index >= 0 && index < _items.size())
            return _items.takeAt(index);
        return nullptr;
    }

    int AreaBinaryLayout::count() const
    {
        return (int)_items.size();
    }

    void AreaBinaryLayout::setGeometry(const QRect& rect)
    {
        if (count() < 3)
            return;

        QLayoutItem* a = _items.at(0);
        QLayoutItem* s = _items.at(1);
        QLayoutItem* b = _items.at(2);

        int x1, y1, x2, y2;

        rect.adjusted(-1, -1, 1, 1)
            .getCoords(&x1, &y1, &x2, &y2);

        _split = std::clamp<qreal>(_split, 0, 1);

        if (isZero((float)_split))
        {
            showLayoutItem(a, false);
            showLayoutItem(b, true);
        }
        else if (equals((float)_split, 1))
        {
            showLayoutItem(a, true);
            showLayoutItem(b, false);
        }
        else
        {
            showLayoutItem(a, true);
            showLayoutItem(b, true);
        }

        if (_orientation == Qt::Vertical)
        {
            // top, bottom split
            const int baseHeight = (y2 - y1 - (Const::SplitterSize));

            int ya = (int)(baseHeight * _split);
            int yb = (int)(baseHeight * (1.0 - _split));

            if (ya + yb < baseHeight)
                yb += (baseHeight - (ya + yb));
            int xs = x2 - x1;

            a->setGeometry({x1, y1, xs, ya});
            y1 += ya;
            s->setGeometry({x1, y1, xs, Const::SplitterSize});
            y1 += Const::SplitterSize;
            b->setGeometry({x1, y1, xs, yb});
        }
        else
        {
            // left, right split
            const int baseHeight = (x2 - x1 - Const::SplitterSize);

            int xa = (int)(baseHeight * _split);
            int xb = (int)(baseHeight * (1.0 - _split));
            int ys = y2 - y1;

            a->setGeometry({x1, y1, xa, ys});
            x1 += xa;
            s->setGeometry({x1, y1, Const::SplitterSize, ys});
            x1 += Const::SplitterSize;
            b->setGeometry({x1, y1, xb, ys});
        }
    }

    void AreaBinaryLayout::setRatio(const qreal ratio)
    {
        _split = ratio;
        invalidate();
    }

    void AreaBinaryLayout::construct(AreaNode* left, AreaNode* right)
    {
        View::layoutDefaults(this);

        if (left)
        {
            addWidget(left);
            if (right)
                addWidget(new AreaBinaryLayoutPrivate(this));
        }
        if (right)
            addWidget(right);
    }

    void AreaBinaryLayout::drag(const QPointF& loc)
    {
        if (_orientation == Qt::Horizontal)
            _split = loc.x();
        else
            _split = loc.y();

        if (_split < 0.0)
            _split = 0.0;
        if (_split > 1.0)
            _split = 1.0;

        invalidate();
    }

}  // namespace Jam::Editor
