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

#include <QMainWindow>
#include "RememberLastCache.h"
#include "Utils/Path.h"

class QProcess;

namespace Jam::Editor
{
    class WindowMenuBar;
    class OutputArea;
    class OutputContext;
    class MainArea;
    class ProjectContext;

    class Application final : public QMainWindow
    {
        Q_OBJECT
    signals:
        void clearContext();

    public:
        enum Flags
        {
            NoFlag     = 0,
            NoSettings = 0x01,
        };

    private:
        MainArea*         _mainArea{nullptr};
        WindowMenuBar*    _menuBar{nullptr};
        QMenu*            _fileRecent{nullptr};
        const int         _flags;
        RememberLastCache _recentFiles;
        QString           _lastOpenDir{};
        QString           _cachedProjectPath{};

        void construct();

        void updateRecent(const QString& fileName);

        void constructRecentMenu();

        void constructMenuBar();

        void swapLayout(const String& string);

        void loadProjectFromPath(const QString& fileName);

        void saveSettings() const;

        void loadSettings();

        [[deprecated]]
        static bool isNotLoadable(PathUtil& dest, const QString& fileName);
        
        // application events

        void notifyProjectOpened() const;

        void notifyProjectClosed() const;

        [[deprecated]] void notifyProjectStateChanged() const;

        void saveProjectImpl(const QString& path);

    public:
        explicit Application(QWidget* parent = nullptr);
        ~Application() override;

        void post();

        void newProject();
        void openProject();
        void closeProject();
        void saveProject();
        void saveProjectAs();
        void recentItemClicked();
        void viewDocs() const;
        void showAbout();
        void editSettings();
        void toggleMaximize();
        void minimize();

    protected:
        bool event(QEvent* event) override;
    };
}  // namespace Jam::Editor
