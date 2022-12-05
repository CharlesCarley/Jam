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
#include "Interface/Style/Style.h"
#include <QPlainTextEdit>
#include <QPushButton>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>
#include "Interface/Style/Icons.h"
#include "Interface/Style/Palette.h"

namespace Jam::Editor
{

    using Role = QPalette::ColorRole;

    constexpr Role MenuBarBackground = Role::Shadow;
    constexpr Role MenuBarForeground = Role::Text;
    constexpr Role MenuBarIconAccent = Role::Dark;

    constexpr Role MenuBackground = Role::Shadow;
    constexpr Role MenuForeground = Role::Text;
    constexpr Role MenuAccent     = Role::Base;

    constexpr Role ToolMenuBackground = Role::Shadow;
    constexpr Role ToolMenuForeground = Role::Text;
    constexpr Role ToolMenuAccent     = Role::Base;

    constexpr Role SettingsTreeBackground = Role::Base;
    constexpr Role SettingsTreeForeground = Role::Text;
    constexpr Role SettingsTreeAccent     = Role::Mid;

    constexpr Role ProjectTreeBackground = Role::Dark;
    constexpr Role ProjectTreeForeground = Role::Text;
    constexpr Role ProjectTreeAccent     = Role::Base;

    constexpr Role SliderBackground = Role::Shadow;
    constexpr Role SliderBorder     = Role::Window;
    constexpr Role SliderAccent     = Role::Mid;
    constexpr Role SliderForeground = Role::Text;
    constexpr Role SliderStep       = Role::Text;

    constexpr Role CheckBoxBackground = Role::Mid;
    constexpr Role CheckBoxChecked    = Const::G03;
    constexpr Role CheckBoxLabel      = Role::Text;

    constexpr Role AreaToolbarBackground = Role::Mid;
    constexpr Role AreaToolbarForeground = Role::Highlight;
    constexpr Role AreaToolbarAccent     = Role::Midlight;

    constexpr Role AreaBackground = Role::Dark;
    constexpr Role AreaForeground = Role::Text;

    constexpr Role AreaSplitter = Role::Dark;
    constexpr int  SplitterSize = 3;

    constexpr Role AreaIconAccent = MenuBarIconAccent;  // or G01

    void applyColorRoles(
        QWidget*   widget,
        const Role background = Role::NoRole,
        const Role foreground = Role::Text)
    {
        Q_ASSERT(widget);
        widget->setAutoFillBackground(background != Role::NoRole);
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

    void Style::apply(QWidget* widget, const StyleTypes type)
    {
        Q_ASSERT(widget);
        widget->setContentsMargins(0, 0, 0, 0);
        widget->setFocusPolicy(Qt::NoFocus);

        switch (type)
        {
        case ApplicationStyle:
            applyColorRoles(widget, Role::NoRole, Role::HighlightedText);
            break;
        case AreaStyle:
            applyColorRoles(widget, Role::Dark);
            break;
        case AreaGridStyle:
            applyColorRoles(widget, Role::Shadow, Role::Text);
            break;
        case AreaEdgeRectStyle:
            applyColorRoles(widget, AreaSplitter, Role::NoRole);
            break;
        case AreaToolBarStyle:
            applyColorRoles(widget, AreaToolbarBackground, AreaToolbarForeground);
            break;
        case AreaToolLabelStyle:
            applyColorRoles(widget, Role::NoRole, AreaToolbarForeground);
            break;
        case AreaToolButtonStyle:
            applyColorRoles(widget, Role::NoRole, AreaToolbarForeground);
            Palette::setColorRole(widget, Role::AlternateBase, AreaToolbarAccent);
            break;
        case AreaProjectTreeStyle:
            applyColorRoles(widget, ProjectTreeBackground, ProjectTreeForeground);
            Palette::swapColorRole(widget, Role::AlternateBase, ProjectTreeAccent);
            break;
        case QLineEditStyle:
            applyColorRoles(widget, Role::Base, Role::Text);
            break;
        case QCheckBoxStyle:
            applyColorRoles(widget, Role::NoRole, CheckBoxLabel);
            Palette::setAccentRole(widget, Role::Link, CheckBoxChecked);
            Palette::setColorRole(widget, Role::AlternateBase, CheckBoxBackground);
            break;
        case AreaSettingsTreeStyle:
            applyColorRoles(widget, SettingsTreeBackground, SettingsTreeForeground);
            Palette::swapColorRole(widget, Role::AlternateBase, SettingsTreeAccent);
            break;
        case MenuBarStyle:
            applyColorRoles(widget, MenuBarBackground, MenuBarForeground);
            Palette::setColorRole(widget, Role::AlternateBase, MenuBarIconAccent);
            break;
        case AreaMenuItemStyle:
            applyColorRoles(widget, MenuBackground, MenuForeground);
            Palette::setColorRole(widget, Role::AlternateBase, MenuAccent);
            break;
        case AreaToolMenuItemStyle:
            applyColorRoles(widget, ToolMenuBackground, ToolMenuForeground);
            Palette::setColorRole(widget, Role::AlternateBase, ToolMenuAccent);
            break;
        case AreaSliderStyle:
            applyColorRoles(widget, SliderBackground, SliderForeground);
            break;
        case AreaDialogStyle:  // TODO: examine
        case AreaNodeStyle:    // should be empty
        case TransparentStyle:
        default:
            applyColorRoles(widget, Role::NoRole, Role::NoRole);
            break;
        }
    }

    int Style::hint(Hints type)
    {
        switch (type)
        {
        case SplitterSizeHint:
            return SplitterSize;
        case ButtonHeight:
        case ButtonWidth:
            return 24;
        default:
            break;
        }
        return -1;
    }

    void Style::sliderPalette(QPalette& palette)
    {
        // SliderBackground = shadow
        // SliderBorder = window
        // SliderAccent = mid
        // constexpr Role SliderBackground = Role::Shadow;
        // constexpr Role SliderBorder     = Role::Window;
        // constexpr Role SliderAccent     = Role::Mid;
        // constexpr Role SliderForeground = Role::Text;
        // constexpr Role SliderStep       = Role::Text;
        palette.setColor(Role::BrightText, palette.color(Role::Text));
    }

    void Style::treeWidgetDefaults(QTreeWidget* widget)
    {
        Q_ASSERT(widget);
        widget->setColumnCount(1);
        widget->setHeaderHidden(true);

        widget->setContentsMargins(0, 0, 0, 0);
        widget->setFocusPolicy(Qt::NoFocus);
        widget->setFrameShape(QFrame::NoFrame);

        const QAbstractScrollArea* scrollArea = (QAbstractScrollArea*)widget;
        if (QWidget* viewport = scrollArea->viewport())
            copyColorRoles(viewport, widget);
    }

    QTreeWidget* Style::treeWidget(const StyleTypes type, QWidget* parent)
    {
        QTreeWidget* widget = new QTreeWidget(parent);
        apply(widget, type);
        treeWidgetDefaults(widget);
        return widget;
    }

    void Style::layoutDefaults(QLayout* widget, int margin, int spacing)
    {
        widget->setSpacing(spacing);
        widget->setContentsMargins(margin, margin, margin, margin);
        widget->setSizeConstraint(QLayout::SetMinAndMaxSize);
    }

    QVBoxLayout* Style::verticalLayout(const int margin,
                                       const int spacing,
                                       QWidget*  parent)
    {
        QVBoxLayout* widget = new QVBoxLayout(parent);
        layoutDefaults(widget, margin, spacing);
        return widget;
    }

    QHBoxLayout* Style::horizontalLayout(const int margin,
                                         const int spacing,
                                         QWidget*  parent)
    {
        QHBoxLayout* widget = new QHBoxLayout(parent);
        layoutDefaults(widget, margin, spacing);
        return widget;
    }

    QHBoxLayout* Style::paddedArea(QWidget* child, const int margin)
    {
        QHBoxLayout* widget = horizontalLayout(margin);
        widget->addWidget(child);
        return widget;
    }

    QPushButton* Style::toolButton(const Icons::Icon ico, QWidget* parent)
    {
        QPushButton* button = new QPushButton(parent);
        button->setIcon(get(ico));

        button->setMaximumSize({
            hint(ButtonWidth),
            hint(ButtonHeight),
        });
        button->setFlat(true);
        apply(button, AreaToolButtonStyle);
        return button;
    }

    QPlainTextEdit* Style::plainText(QWidget* parent)
    {
        const auto edit = new QPlainTextEdit(parent);
        edit->setWordWrapMode(QTextOption::NoWrap);
        edit->setBackgroundVisible(false);
        edit->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        edit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        edit->setReadOnly(true);
        return edit;
    }
}  // namespace Jam::Editor
