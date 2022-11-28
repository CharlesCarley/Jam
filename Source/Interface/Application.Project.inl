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
#include <QDir>
#include <QFileDialog>
#include "State/ProjectManager.h"
#include "Utils/Path.h"

namespace Jam::Editor
{
    bool Application::isNotLoadable(PathUtil& dest, const QString& fileName)
    {
        if (fileName.isEmpty())
            return true;

        dest = PathUtil(fileName.toStdString());
        return !dest.exists();
    }

    void Application::updateRecent(const QString& fileName)
    {
        _recentFiles.store(fileName);

        constructRecentMenu();
    }

    void Application::loadProjectFromPath(const QString& fileName)
    {
        if (fileName.isEmpty())  // unlikely
        {
            Console::writeLine("WTF, the supplied filename is empty!");
            return;
        }
        const PathUtil path(fileName.toStdString());
        if (!path.exists())  // likely
        {
            Console::writeLine(
                "the supplied filename does "
                "not exist or is inaccessible. '",
                path.fullPath(),
                ',');
            return;
        }

        if (const auto state = State::projectState())
        {
            if (state->load(path.fullPath()))
            {
                updateRecent(fileName);

                swapLayout(state->layout());

                // save the current path information

                _lastOpenDir       = path.rootedDir().c_str();
                _cachedProjectPath = path.fullPath().c_str();

                // relay the state change to all listeners.
                notifyProjectOpened();
            }
        }
    }

    void Application::newProject()
    {
        if (State::ProjectManager* state = State::projectState())
            state->unload();

        _cachedProjectPath = {};

        PersistentSettings().ref().lastProject = {};

        _mainArea->clear();
        State::projectState()->loadDefaultStack();
        notifyProjectOpened();
    }

    void Application::openProject()
    {
        // Seed the last open directory persistent state.
        // if this is empty, then it was not found when load settings
        // was called, in this case set it from the cwd and go from there.
        if (_lastOpenDir.isEmpty())
            _lastOpenDir = QDir::currentPath();

        if (const QString fileName = QFileDialog::getOpenFileName(
                this,
                "Open Project",
                _lastOpenDir,
                "Jam Project Files (*.jam *.xml)");
            !fileName.isEmpty())
        {
            loadProjectFromPath(fileName);
        }
        else
        {
            Console::writeLine(
                "failed to extract a valid "
                "file name via the file dialog.");
        }
    }

    void Application::closeProject()
    {
        if (State::ProjectManager* state = State::projectState())
            state->unload();

        _cachedProjectPath = {};

        PersistentSettings().ref().lastProject = {};

        _mainArea->clear();

        // send a ProjectClosed event
        notifyProjectClosed();
    }

    void Application::saveProjectImpl(const QString& path)
    {
        if (State::ProjectManager* state = State::projectState())
        {
            String layout;
            _mainArea->serialize(layout);

            if (!state->saveAs(
                    path.toStdString(),
                    layout))
            {
                Console::writeLine(
                    "failed to save the the cached project file: '",
                    path.toStdString(),
                    "'");
            }
            else
            {
                _cachedProjectPath = path;
            }
        }
    }

    void Application::saveProject()
    {
        if (_cachedProjectPath.isEmpty())
            saveProjectAs();
        else
        {
            // _cachedProjectPath is reassigned...
            saveProjectImpl(_cachedProjectPath);
        }
    }

    void Application::saveProjectAs()
    {
        if (const QString fileName =
                QFileDialog::getSaveFileName(
                    this,
                    "Save Project",
                    _lastOpenDir,
                    "Jam Project Files (*.jam)");
            !fileName.isEmpty())
        {
            saveProjectImpl(fileName);
        }
        else
        {
            Console::writeLine(
                "failed to extract a valid "
                "file name via the file dialog.");
        }
    }
}  // namespace Jam::Editor
