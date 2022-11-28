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
#include "Interface/Widgets/IconButton.h"
#include <QPushButton>
#include "Interface/Constants.h"
#include "Interface/Extensions.h"

namespace Jam::Editor
{
    IconButton::IconButton(QWidget* parent) :
        QPushButton(parent)
    {
    }

    IconButton* IconButton::createButton(const Icons::Icon& icon, QWidget* parent)
    {
        IconButton* button = new IconButton(parent);

        View::widgetDefaults(button, Const::IconPadding);
        View::emptyWidget(button);

        // force the size of the icon
        button->setMaximumSize({24, 24});

        button->setFlat(true);
        button->setIcon(get(icon));
        return button;
    }

    IconButton* IconButton::createToolButton(const Icons::Icon& icon, QWidget* parent)
    {
        IconButton* button = createButton(icon, parent);
        button->setProperty("iconType", 1);
        return button;
    }

    IconButton* IconButton::createTitleButton(const Icons::Icon& icon, QWidget* parent)
    {
        IconButton* button = createButton(icon, parent);
        button->setProperty("iconType", 2);
        return button;
    }

    QIcon Icons::get(const Icon icon)
    {
        switch (icon)
        {
        case Add:
            return QIcon(":/icons/ic_add.svg");
        case ArrowBack:
            return QIcon(":/icons/ic_arrow_back.svg");
        case ArrowDownward:
            return QIcon(":/icons/ic_arrow_downward.svg");
        case ArrowDropDown:
            return QIcon(":/icons/ic_arrow_drop_down.svg");
        case ArrowDropUp:
            return QIcon(":/icons/ic_arrow_drop_up.svg");
        case ArrowForward:
            return QIcon(":/icons/ic_arrow_forward.svg");
        case ArrowUpward:
            return QIcon(":/icons/ic_arrow_upward.svg");
        case BugReport:
            return QIcon(":/icons/ic_bug_report.svg");
        case Check:
            return QIcon(":/icons/ic_check.svg");
        case CheckBox:
            return QIcon(":/icons/ic_check_box.svg");
        case CheckOff:
            return QIcon(":/icons/ic_check_off.svg");
        case CheckOn:
            return QIcon(":/icons/ic_check_on.svg");
        case Clear:
            return QIcon(":/icons/ic_clear.svg");
        case Close:
            return QIcon(":/icons/ic_close.svg");
        case Create:
            return QIcon(":/icons/ic_create.svg");
        case Cube:
            return QIcon(":/icons/ic_cube.svg");
        case Delete:
            return QIcon(":/icons/ic_delete.svg");
        case Drop:
            return QIcon(":/icons/ic_drop.svg");
        case EditSettings:
            return QIcon(":/icons/ic_edit_settings.svg");
        case File:
            return QIcon(":/icons/ic_file.svg");
        case Folder:
            return QIcon(":/icons/ic_folder.svg");
        case FolderOpen:
            return QIcon(":/icons/ic_folder_open.svg");
        case Fullscreen:
            return QIcon(":/icons/ic_fullscreen.svg");
        case FullscreenExit:
            return QIcon(":/icons/ic_fullscreen_exit.svg");
        case Gamepad:
            return QIcon(":/icons/ic_gamepad.svg");
        case Graph:
            return QIcon(":/icons/ic_graph.svg");
        case GraphArea:
            return QIcon(":/icons/ic_graph_area.svg");
        case GraphBar:
            return QIcon(":/icons/ic_graph_bar.svg");
        case GraphLine:
            return QIcon(":/icons/ic_graph_line.svg");
        case Help:
            return QIcon(":/icons/ic_help.svg");
        case Home:
            return QIcon(":/icons/ic_home.svg");
        case Link:
            return QIcon(":/icons/ic_link.svg");
        case Maximize:
            return QIcon(":/icons/ic_maximize.svg");
        case Menu:
            return QIcon(":/icons/ic_menu.svg");
        case Minimize:
            return QIcon(":/icons/ic_minimize.svg");
        case MoreHorizontal:
            return QIcon(":/icons/ic_more_horizontal.svg");
        case MoreVertical:
            return QIcon(":/icons/ic_more_vertical.svg");
        case Output:
            return QIcon(":/icons/ic_output.svg");
        case Quit:
            return QIcon(":/icons/ic_quit.svg");
        case Refresh:
            return QIcon(":/icons/ic_refresh.svg");
        case Save:
            return QIcon(":/icons/ic_save.svg");
        case SelectAll:
            return QIcon(":/icons/ic_select_all.svg");
        case Settings:
            return QIcon(":/icons/ic_settings.svg");
        case Share:
            return QIcon(":/icons/ic_share.svg");
        case Sync:
            return QIcon(":/icons/ic_sync.svg");
        case TrendingFlat:
            return QIcon(":/icons/ic_trending_flat.svg");
        case Undo:
            return QIcon(":/icons/ic_undo.svg");
        case Update:
            return QIcon(":/icons/ic_update.svg");
        case Visibility:
            return QIcon(":/icons/ic_visibility.svg");
        case VisibilityOff:
            return QIcon(":/icons/ic_visibility_off.svg");
        case Window:
            return QIcon(":/icons/ic_window.svg");
        case XIcon:
            return QIcon(":/icons/ic_xicon.svg");
        default:
            return {};
        }
    }
}  // namespace Jam::Editor
