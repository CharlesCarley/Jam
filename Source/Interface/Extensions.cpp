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
#include "Interface/Extensions.h"
#include <QApplication>
#include <QBoxLayout>
#include <QCoreApplication>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QWidget>
#include "Interface/Area/AreaBranch.h"
#include "Interface/Area/AreaLeaf.h"
#include "Utils/Array.h"

namespace Jam::Editor::View
{
    void windowRect(QRect& winRect, const QWidget* widget)
    {
        Q_ASSERT(widget);

        if (const QWidget* win = widget ? widget->window() : nullptr)
            winRect = win->geometry();
        else
            winRect = {};
    }

    void constrainToScreen(QWidget* widget)
    {
        Q_ASSERT(widget);
        if (const QScreen* sc =
                widget
                    ? QApplication::screenAt(widget->pos())
                    : nullptr)
        {
            const QSizeF screen = sc->size();

            widget->setMinimumSize(
                int(screen.width() / 2.5),
                int(screen.height() / 2.5));

            widget->setFixedSize(
                int(screen.width() / 1.75),
                int(screen.height() / 1.75));

            widget->setMaximumSize(
                int(screen.width()),
                int(screen.height()));
        }
    }

    void contractRect(QRect& winRect, const int scale)
    {
        winRect.adjust(scale, scale, -scale, -scale);
    }
    


    bool isBranch(const QWidget* widget)
    {
        return widget && widget->inherits(AreaBranch::staticMetaObject.className());
    }

    bool isLeaf(const QWidget* widget)
    {
        return widget && widget->inherits(AreaLeaf::staticMetaObject.className());
    }

    void clearLayout(QLayout* layout, SimpleArray<QWidget*>& dangled)
    {
        Q_ASSERT(layout);

        dangled.clear();
        while (const QLayoutItem* item = layout->takeAt(0))
        {
            if (QWidget* widget = item->widget())
            {
                widget->setParent(nullptr);
                dangled.push_back(widget);
            }
            delete item;
        }
    }

    QSize calcMaxHeight(const QLayout* layout)
    {
        const QMargins m = layout->contentsMargins();
        const int      s = layout->spacing();

        int h = m.top() + m.bottom(), w = 0;
        if (layout)
        {
            int i = 0;
            while (i < layout->count())
            {
                const auto item = layout->itemAt(i++);
                if (const auto panel = item->widget())
                {
                    QSize sh = panel->sizeHint();
                    h += sh.height();
                    w = Max(w, sh.width());
                    if (i < layout->count())
                        h += s;
                }
            }
        }
        return {w, h};
    }
}  // namespace Jam::Editor::View
