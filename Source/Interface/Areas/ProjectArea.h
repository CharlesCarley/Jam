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
#include <qtreewidget.h>
#include "State/ProjectManager.h"
#include "Utils/String.h"
#include "Interface/Area/Area.h"
#include "Xml/Node.h"

namespace Jam
{
    namespace Editor
    {
        namespace State
        {
            class FrameStackManager;
        }
    }
}

class QTreeWidget;
class QFileSystemWatcher;
class QTextEdit;

namespace Jam::Editor
{
    class ProjectArea final : public Area
    {
        Q_OBJECT
    private:
        QTreeWidget* _tree{nullptr};
        String       _directoryRoot;
        String       _fullFilePath;

        void construct();

        void constructToolbar();

        void constructTree(const State::FrameStackManager* stack) const;

        void setState() const;

        void showProjectSettings();

        bool event(QEvent* event) override;

        void itemDoubleClicked(const QTreeWidgetItem * item, int column) const;

    public:
        explicit ProjectArea(AreaCreator *creator, QWidget* parent = nullptr);
        ~ProjectArea() override;
    };

}  // namespace Jam::Editor
