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
#ifndef APPLICATION_INLINE_DEFINITION
    #error This file should not be included here.
#endif

namespace Jam::Editor
{
    void Application::saveSettings() const
    {
        if (_flags & NoSettings)
            return;

        // Log::writeLine("Saving settings");

        const PersistentSettings settings;

        if (isMaximized())
        {
            settings.setGeometry(QRect{0, 0, 800, 600});
            settings.setMaximize(true);
        }
        else
        {
            settings.setGeometry(this->geometry());
            settings.setMaximize(false);
        }

        settings.setLastOpenDirectory(_lastOpenDir);
        settings.setRecentFiles(_recentFiles.data());

        if (settings.reloadLast())
            settings.setLastProject(_cachedProjectPath);
        else
            settings.setLastProject({});
    }

    void Application::loadSettings()
    {
        // Reload any application specific user settings.
        // things like window state, size and location.
        // Restrict to only window state data..
        if (_flags & NoSettings)
            return;

        Log::clear();
        // Log::writeLine("Loading settings");

        const PersistentSettings settings;

        setGeometry(settings.geometry(QRect{0, 0, 800, 600}));

        if (settings.maximize())
            setWindowState(Qt::WindowMaximized);
        else
            setWindowState(Qt::WindowNoState);

        _recentFiles = RememberLastCache(settings.recentFiles());

        // used to remember last open directory for the file dialog.
        _lastOpenDir = settings.lastOpenDirectory();

        _cachedProjectPath = settings.lastProject();
    }

}  // namespace Jam::Editor
