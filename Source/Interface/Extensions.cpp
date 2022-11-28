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
#include <QPlainTextEdit>
#include <QPushButton>
#include <QTreeWidget>
#include <QWidget>
#include "Interface/Area/AreaBranch.h"
#include "Interface/Area/AreaLeaf.h"
#include "Interface/Constants.h"
#include "Utils/Array.h"

namespace Jam::Editor::View
{
    void windowRect(QRect& winRect, const QWidget* widget)
    {


        if (const QWidget* win = widget ? widget->window() : nullptr)
            winRect = win->geometry();
        else
            winRect = {};
    }

    void dialogDefaults(QWidget* widget)
    {
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

    void applyColorRoles(
        QWidget*    widget,
        const QRole background,
        const QRole foreground)
    {
        Q_ASSERT(widget);
        widget->setAutoFillBackground(background != QRole::NoRole);
        widget->setForegroundRole(foreground);
        widget->setBackgroundRole(background);
    }

    void copyColorRoles(QWidget* dst, const QWidget* src)
    {
        Q_ASSERT(src && dst);
        dst->setAutoFillBackground(src->autoFillBackground());
        dst->setForegroundRole(src->foregroundRole());
        dst->setBackgroundRole(src->backgroundRole());
    }

    void clearTextRole(QWidget* widget, const QRole foreground)
    {
        Q_ASSERT(widget);
        widget->setAutoFillBackground(false);
        widget->setForegroundRole(foreground);
        widget->setBackgroundRole(QRole::NoRole);
    }

    void applyBaseClassDefaults(QWidget* widget)
    {
        Q_ASSERT(widget);

        if (widget->inherits(QFrame::staticMetaObject.className()))
        {
            QFrame* frame = (QFrame*)widget;
            frame->setFrameShape(QFrame::NoFrame);
        }

        if (widget->inherits(QAbstractScrollArea::staticMetaObject.className()))
        {
            const QAbstractScrollArea* scrollArea = (QAbstractScrollArea*)widget;
            if (QWidget* viewport = scrollArea->viewport())
                copyColorRoles(viewport, widget);
        }

        if (widget->inherits(QPlainTextEdit::staticMetaObject.className()))
        {
            QPlainTextEdit* edit = (QPlainTextEdit*)widget;

            edit->setWordWrapMode(QTextOption::NoWrap);
            edit->setBackgroundVisible(false);
            edit->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
            edit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
            edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            edit->setReadOnly(true);
        }
    }

    void widgetDefaults(QWidget* widget, int margins, const QWidget* parent)
    {
        Q_ASSERT(widget);
        widget->setContentsMargins(margins, margins, margins, margins);
        widget->setFocusPolicy(Qt::NoFocus);
        if (parent)
            copyColorRoles(widget, parent);
        applyBaseClassDefaults(widget);
    }

    void widgetDefaults(QWidget* widget, const QWidget* parent)
    {
        Q_ASSERT(widget && parent);

        widget->setContentsMargins(0, 0, 0, 0);
        copyColorRoles(widget, parent);
        applyBaseClassDefaults(widget);
    }

    void layoutDefaults(QLayout* dst, int margins, int spacing)
    {
        Q_ASSERT(dst);
        dst->setSpacing(spacing);
        dst->setContentsMargins(margins, margins, margins, margins);
        dst->setSizeConstraint(QLayout::SetMinAndMaxSize);
    }

    void emptyWidget(QWidget* dst)
    {
        widgetDefaults(dst);
        applyColorRoles(dst, QRole::NoRole, QRole::NoRole);
    }

    void menuRole(QWidget* dst)
    {
        applyColorRoles(dst, Const::MenuRole, QRole::Text);
    }

    void treeWidgetDefaults(QTreeWidget* dst)
    {
        dst->setColumnCount(1);
        dst->setHeaderHidden(true);

        widgetDefaults(dst);
        applyColorRoles(dst, QRole::NoRole, QRole::Text);
    }

    void treeWidgetDefaults(QTreeWidget* dst, const QWidget* parent)
    {
        dst->setColumnCount(1);
        dst->setHeaderHidden(true);

        widgetDefaults(dst, parent);
    }

    void pushButtonDefaults(QPushButton* dst)
    {
        widgetDefaults(dst);
        applyColorRoles(dst, QRole::Button, QRole::Text);

        dst->setMinimumSize({18, 24});
    }

    void addLayoutMargin(QBoxLayout* dst, QWidget* content, int margin)
    {
        const auto innerLayout = new QHBoxLayout();
        innerLayout->setSpacing(0);
        innerLayout->setContentsMargins(margin, margin, margin, margin);

        innerLayout->addWidget(content, 1);
        dst->addLayout(innerLayout, 1);
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
}  // namespace Jam::Editor
