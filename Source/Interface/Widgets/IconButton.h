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
#include <QIcon>
#include <QPushButton>
#include "OkCancelWidget.h"

class QWidget;
class QPushButton;

namespace Jam::Editor
{
    namespace Icons
    {
        enum Icon
        {
            Add,
            ArrowBack,
            ArrowDownward,
            ArrowDropDown,
            ArrowDropUp,
            ArrowForward,
            ArrowUpward,
            BugReport,
            Check,
            CheckBox,
            CheckOff,
            CheckOn,
            Clear,
            Close,
            Create,
            Cube,
            Delete,
            Drop,
            EditSettings,
            File,
            Folder,
            FolderOpen,
            Fullscreen,
            FullscreenExit,
            Gamepad,
            Graph,
            GraphArea,
            GraphBar,
            GraphLine,
            Help,
            Home,
            Link,
            Maximize,
            Menu,
            Minimize,
            MoreHorizontal,
            MoreVertical,
            Output,
            Quit,
            Refresh,
            Save,
            SelectAll,
            Settings,
            Share,
            Sync,
            TrendingFlat,
            Undo,
            Update,
            Visibility,
            VisibilityOff,
            Window,
            XIcon,
        };
        extern QIcon get(Icon icon);
    }  // namespace Icons

    class IconButton final : public QPushButton
    {

        Q_OBJECT
        Q_PROPERTY(int iconType MEMBER _type)
    private:
        int _type{0};

        explicit IconButton(QWidget* parent = nullptr);



        static IconButton* createButton(
            const Icons::Icon& icon,
            QWidget*           parent);


    public:
        static IconButton* createToolButton(
            const Icons::Icon& icon,
            QWidget*           parent = nullptr);

        static IconButton* createTitleButton(
            const Icons::Icon& icon,
            QWidget*           parent = nullptr);
    };

}  // namespace Jam::Editor
