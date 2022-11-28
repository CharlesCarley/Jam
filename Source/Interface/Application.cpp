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
#include "Interface/Application.h"
#include "Interface/Areas/OutputArea.h"
#include "Interface/Extensions.h"
#include "Interface/MainArea.h"
#include "Interface/Widgets/WindowMenuBar.h"

namespace Jam::Editor
{
    Application::Application(QWidget* parent) :
        QMainWindow(parent),
        _flags{NoFlag}
    {
        construct();
    }

    Application::~Application()
    {
        (void)disconnect(this);

        delete _menuBar;
        _menuBar = nullptr;

        saveSettings();
        delete _mainArea;
    }

    void Application::construct()
    {
        // Update the application window settings
        loadSettings();

        setWindowFlags(Qt::FramelessWindowHint);
        setMinimumSize(320, 480);

        View::widgetDefaults(this);
        View::applyColorRoles(this, QPalette::NoRole, QPalette::HighlightedText);

        constructMenuBar();

        // Reload the last state of the working project.
        if (const QString& lastOpen = _cachedProjectPath;
            !lastOpen.isEmpty())
            loadProjectFromPath(lastOpen);

        if (!_mainArea)
        {
            // This pulls the layout from the application settings.
            // The layout needs to be loaded from the project file instead.
            _mainArea = new MainArea();
            // Hand it Qt
            setCentralWidget(_mainArea);

            newProject();
        }
    }

    void Application::post()
    {
        if (windowState() == Qt::WindowMaximized)
            showMaximized();
        else
            show();
    }

    void Application::swapLayout(const String& string)
    {
        if (_mainArea)
        {
            _mainArea->setParent(nullptr);
            setCentralWidget(nullptr);
            delete _mainArea;
        }

        _mainArea = new MainArea(string.c_str(), this);
        setCentralWidget(_mainArea);
    }

    void Application::toggleMaximize()
    {
        if (windowState() == Qt::WindowMaximized)
            setWindowState(Qt::WindowNoState);
        else
            setWindowState(Qt::WindowMaximized);
    }

    void Application::minimize()
    {
        setWindowState(Qt::WindowMinimized);
    }

}  // namespace Jam::Editor

#include "Application.Dialogs.inl"
#include "Application.Events.inl"
#include "Application.Menu.inl"
#include "Application.Project.inl"
#include "Application.Settings.inl"
