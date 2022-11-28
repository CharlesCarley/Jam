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
#include "Interface/Widgets/IconButton.h"
#include "Interface/Widgets/WindowMenuBar.h"

namespace Jam::Editor
{
    inline bool isFilePresent(const QString& str)
    {
        const PathUtil pathUtil(str.toStdString());
        return pathUtil.exists();
    }

    void Application::constructMenuBar()
    {
        // -------- [ MenuBar ] --------

        _menuBar = new WindowMenuBar();
        setMenuBar(_menuBar);

        QMenu* file = _menuBar->addMenu("File");
        QMenu* edit = _menuBar->addMenu("Edit");
        QMenu* view = _menuBar->addMenu("View");
        QMenu* help = _menuBar->addMenu("Help");

        View::menuRole(file);
        View::menuRole(edit);
        View::menuRole(view);
        View::menuRole(help);

        // -------- [ File ] --------

        QAction* fileQuit = new QAction("Quit");
        fileQuit->setIcon(get(Icons::Quit));
        fileQuit->setShortcut(QKeySequence("Ctrl+Q"));
        connect(fileQuit, &QAction::triggered, this, QApplication::quit);

        QAction* fileNewProject = new QAction("New Project");
        fileNewProject->setShortcut(QKeySequence("Ctrl+N"));
        connect(fileNewProject, &QAction::triggered, this, &Application::newProject);

        QAction* fileOpenProject = new QAction("Open");
        fileOpenProject->setShortcut(QKeySequence("Ctrl+O"));
        connect(fileOpenProject, &QAction::triggered, this, &Application::openProject);

        QAction* fileSaveProject = new QAction("Save");
        fileSaveProject->setShortcut(QKeySequence("Ctrl+S"));
        fileSaveProject->setIcon(get(Icons::Save));
        connect(fileSaveProject, &QAction::triggered, this, &Application::saveProject);

        QAction* fileCloseProject = new QAction("Close");
        connect(fileCloseProject, &QAction::triggered, this, &Application::closeProject);

        QAction* fileSaveAsProject = new QAction("Save As");
        fileSaveAsProject->setShortcut(QKeySequence("Ctrl+Shift+S"));
        connect(fileSaveAsProject, &QAction::triggered, this, &Application::saveProjectAs);

        file->addAction(fileNewProject);
        file->addAction(fileOpenProject);
        file->addSeparator();
        file->addAction(fileCloseProject);
        file->addSeparator();
        file->addAction(fileSaveProject);
        file->addAction(fileSaveAsProject);
        file->addSeparator();

        _fileRecent = new QMenu("Recent Projects");
        constructRecentMenu();

        file->addMenu(_fileRecent);
        file->addSeparator();
        file->addAction(fileQuit);

        // -------- [ Edit ] --------

        QAction* editPreferences = new QAction("Preferences");
        editPreferences->setShortcut(QKeySequence("Alt+P"));
        editPreferences->setIcon(get(Icons::EditSettings));
        connect(editPreferences, &QAction::triggered, this, &Application::editSettings);

        edit->addAction(editPreferences);

        // -------- [ View ] --------
        view->addAction("Dump Layout", [=]
                        {
                            if (_mainArea)
                                _mainArea->dumpDisplayTree(); });

        // -------- [ Help ] --------

        QAction* helpAbout = new QAction("About");
        helpAbout->setIcon(get(Icons::Help));
        connect(helpAbout, &QAction::triggered, this, &Application::showAbout);

        help->addAction(helpAbout);

        connect(_menuBar, &WindowMenuBar::exit, this, [=]
                { close(); });

        connect(_menuBar, &WindowMenuBar::maximize, this, [=]
                { toggleMaximize(); });

        connect(_menuBar, &WindowMenuBar::minimize, this, [=]
                { minimize(); });
    }

    void Application::constructRecentMenu()
    {
        if (!_fileRecent)
            return;

        // First remove all elements in the menu.
        _fileRecent->clear();

        for (int i = 0; i < _recentFiles.size(); ++i)
        {
            const QString path = _recentFiles.at(i);

            // Check for invalid data..
            if (path.isEmpty())
                continue;
            QAction* recent = new QAction(
                PathUtil(
                    path.toStdString())
                    .fileName()
                    .c_str());
            recent->setData(path);

            connect(recent,
                    &QAction::triggered,
                    this,
                    &Application::recentItemClicked);

            _fileRecent->addAction(recent);
        }

        _fileRecent->addSeparator();
        QAction* fileClearRecent = new QAction("Clear recent files");
        fileClearRecent->setIcon(get(Icons::Clear));

        connect(fileClearRecent, &QAction::triggered, this, [=]
                {
                    if (_fileRecent)
                        _fileRecent->clear();
                    _recentFiles.clear(); });
        _fileRecent->addAction(fileClearRecent);
    }

    void Application::recentItemClicked()
    {
        if (const QAction* clickedItem = (QAction*)sender())
        {
            if (const QString clickedItemData = clickedItem->data().toString();
                isFilePresent(clickedItemData))
                loadProjectFromPath(clickedItemData);
            else
            {
                Log::writeLine("File ", 
                    clickedItemData.toStdString(), 
                    " does not exist.");
                _recentFiles.validate();
            }
        }
    }

}  // namespace Jam::Editor
