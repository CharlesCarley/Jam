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
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
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

    constexpr Role LabelBackground = Role::NoRole;
    constexpr Role LabelForeground = Role::Text;

    constexpr Role PushButtonBackground = Role::Button;
    constexpr Role PushButtonForeground = Role::ButtonText;
    constexpr Role PushButtonHover      = Role::Midlight;

    constexpr Role SliderBackground = Role::NoRole;
    constexpr Role SliderForeground = Role::Text;

    constexpr Role CheckBoxBackground = Role::Mid;
    constexpr Role CheckBoxChecked    = Const::G03;
    constexpr Role CheckBoxLabel      = Role::Text;

    constexpr Role AreaToolbarBackground = Role::Mid;
    constexpr Role AreaToolbarForeground = Role::Highlight;
    constexpr Role AreaToolbarAccent     = Role::Midlight;
    constexpr Role AreaSplitter          = Role::Dark;

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

        switch (type)
        {
        case ApplicationStyle:
            applyColorRoles(widget, Role::NoRole, Role::HighlightedText);
            break;
        case AreaDialogStyle:
        case AreaStyle:
        case AreaDialogContainerStyle:
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
            buttonDefaults(widget);
            break;
        case AreaLabelStyle:
            applyColorRoles(widget, LabelBackground, LabelForeground);
            buttonDefaults(widget);
            break;
        case AreaPushButtonStyle:
            applyColorRoles(widget, PushButtonBackground, PushButtonForeground);
            Palette::setColorRole(widget, Role::AlternateBase, PushButtonHover);
            pushButtonDefaults(widget);
            break;
        case AreaToolButtonStyle:
            applyColorRoles(widget, Role::NoRole, AreaToolbarForeground);
            Palette::setColorRole(widget, Role::AlternateBase, AreaToolbarAccent);
            break;
        case AreaProjectTreeStyle:
            applyColorRoles(widget, ProjectTreeBackground, ProjectTreeForeground);
            Palette::swapColorRole(widget, Role::AlternateBase, ProjectTreeAccent);
            break;
        case AreaLineStyle:
            applyColorRoles(widget, Role::Base, Role::Text);
            buttonDefaults(widget);
            break;
        case AreaDarkLineStyle:
            applyColorRoles(widget, Role::Shadow, Role::Text);
            buttonDefaults(widget);
            break;
        case AreaCheckBoxStyle:
            applyColorRoles(widget, Role::NoRole, CheckBoxLabel);
            Palette::setAccentRole(widget, Role::Link, CheckBoxChecked);
            Palette::setColorRole(widget, Role::AlternateBase, CheckBoxBackground);
            buttonDefaults(widget);
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
            buttonDefaults(widget);
            break;
        case AreaNodeStyle:  // should be empty
        case TransparentStyle:
        default:
            applyColorRoles(widget, Role::NoRole, Role::NoRole);
            break;
        }
    }

    int Style::hint(const Hints type)
    {
        switch (type)
        {
        case ToolbarSpacing:
            return 10;
        case SplitterSize:
            return 4;
        case ButtonHeight:
        case ButtonWidth:
        case PushButtonWidth:
            return 24;
        case PushButtonHeight:
            return 18;
        default:
            break;
        }
        return -1;
    }

    void Style::sliderPalette(QPalette& palette)
    {
        // Theses are accessed through R32WidgetSlider::palette
        // methods when rendering .
        // constexpr Role SliderBackground = shadow()
        // constexpr Role SliderBorder     = window()
        // constexpr Role SliderAccent     = mid()
        // constexpr Role SliderForeground = text()
        // constexpr Role SliderStep       = brightText()
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

    void Style::layoutDefaults(QLayout*  widget,
                               const int margin,
                               const int spacing)
    {
        Q_ASSERT(widget);
        widget->setSpacing(spacing);
        widget->setContentsMargins(margin, margin, margin, margin);
        widget->setSizeConstraint(QLayout::SetMinAndMaxSize);
    }

    void Style::buttonDefaults(QWidget* widget)
    {
        Q_ASSERT(widget);
        widget->setMaximumHeight(hint(ButtonHeight));
        widget->setMinimumHeight(hint(ButtonHeight));
        // only constrain the min width..
        widget->setMinimumWidth(hint(ButtonWidth));
    }

    void Style::pushButtonDefaults(QWidget* widget)
    {
        Q_ASSERT(widget);
        widget->setMinimumSize({hint(PushButtonWidth), hint(PushButtonHeight)});
        widget->setFocusPolicy(Qt::FocusPolicy::TabFocus);
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
        Q_ASSERT(child);
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

    QLineEdit* Style::line(QWidget* parent)
    {
        QLineEdit* line = new QLineEdit(parent);

        line->setContentsMargins(0, 0, 0, 0);
        line->setClearButtonEnabled(false);
        line->setReadOnly(false);
        line->setFocusPolicy(Qt::FocusPolicy::StrongFocus);

        apply(line, AreaLineStyle);

        return line;
    }

    QLineEdit* Style::darkLine(QWidget* parent)
    {
        QLineEdit* edit = new QLineEdit(parent);

        edit->setContentsMargins(0, 0, 0, 0);
        edit->setClearButtonEnabled(false);
        edit->setReadOnly(false);
        edit->setFocusPolicy(Qt::FocusPolicy::StrongFocus);

        apply(edit, AreaDarkLineStyle);
        return edit;
    }

    QPushButton* Style::button(const QString& label, QWidget* parent)
    {
        QPushButton* button = new QPushButton(label, parent);

        apply(button, AreaPushButtonStyle);
        return button;
    }

    QLabel* Style::text(const QString& label, QWidget* parent)
    {
        const auto text = new QLabel(label);
        apply(text, AreaLabelStyle);
        return text;
    }

    QCheckBox* Style::checkBox(const QString& label, const bool initial)
    {
        const auto checkBox = new QCheckBox(label);

        apply(checkBox, AreaCheckBoxStyle);

        checkBox->setChecked(initial);
        return checkBox;
    }

}  // namespace Jam::Editor
