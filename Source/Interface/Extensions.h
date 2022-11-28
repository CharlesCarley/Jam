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
#include <QLayout>
#include <QPalette>

#include "Utils/Array.h"

class QWidget;
class QBoxLayout;
class QLayout;
class QTreeWidget;
class QPushButton;

namespace Jam::Editor::View
{
    // this namespace should be used for grouping
    // common setters for QWidget and derived Qt types

    using QRole = QPalette::ColorRole;

    extern void windowRect(QRect& winRect, const QWidget* widget);

    extern void dialogDefaults(QWidget* widget);

    extern void contractRect(QRect& winRect, int scale);

    extern void applyColorRoles(QWidget* widget, QRole background = QRole::NoRole, QRole foreground = QRole::Text);

    extern void copyColorRoles(QWidget* dst, const QWidget* src);

    extern void clearTextRole(QWidget* widget, QRole foreground = QRole::Text);

    extern void widgetDefaults(QWidget* widget, int margins = 0, const QWidget* parent = nullptr);

    extern void widgetDefaults(QWidget* widget, const QWidget* parent);

    extern void layoutDefaults(QLayout* dst, int margins = 0, int spacing = 0);

    extern void emptyWidget(QWidget* dst);

    extern void menuRole(QWidget* dst);

    extern void treeWidgetDefaults(QTreeWidget* dst);

    extern void treeWidgetDefaults(QTreeWidget* dst, const QWidget* parent);

    extern void pushButtonDefaults(QPushButton* dst);

    extern void addLayoutMargin(QBoxLayout* dst, QWidget* content, int margin);

    extern bool isBranch(const QWidget* widget);

    extern bool isLeaf(const QWidget* widget);

    extern void clearLayout(QLayout* layout, SimpleArray<QWidget*>& dangled);
}  // namespace Jam::Editor
