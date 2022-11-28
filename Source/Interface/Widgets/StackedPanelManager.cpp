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
#include "StackedPanelManager.h"
#include <QLayout>
#include "StackedPanel.h"

namespace Jam::Editor
{
    StackedPanelManager::StackedPanelManager() = default;

    StackedPanelManager::~StackedPanelManager()
    {
        while (const QLayoutItem* item = StackedPanelManager::takeAt(0))
        {
            delete item->widget();
            delete item;
        }
        _items.clear();
    }

    void StackedPanelManager::addPanel(StackedPanel* panel)
    {
        addWidget(panel);
    }

    QSize StackedPanelManager::sizeHint() const
    {
        if (const QWidget* widget = parentWidget())
            return widget->size();
        return {};
    }

    void StackedPanelManager::addItem(QLayoutItem* item)
    {
        if (item != nullptr)
            _items.append(item);
    }

    QLayoutItem* StackedPanelManager::itemAt(const int index) const
    {
        if (index >= 0 && index < _items.size())
            return _items.at(index);
        return nullptr;
    }

    QLayoutItem* StackedPanelManager::takeAt(const int index)
    {
        if (index >= 0 && index < _items.size())
            return _items.takeAt(index);
        return nullptr;
    }

    int StackedPanelManager::count() const
    {
        return (int)_items.size();
    }

    void StackedPanelManager::setGeometry(const QRect& rect)
    {
        int       x1, y1, x2, y2;
        qsizetype i = 0;
        const qsizetype n = _items.count();
        rect.adjusted(-1, -1, 1, 1)
            .getCoords(&x1, &y1, &x2, &y2);

        while (i < n)
        {
            const QLayoutItem* item  = _items.at(i);
            StackedPanel*      panel = (StackedPanel*)item->widget();

            int h = panel->sizeHint().height();
            panel->setGeometry({x1, y1, x2 - x1, h});

            y1 += h;
            ++i;
        }
    }
}  // namespace Jam::Editor
