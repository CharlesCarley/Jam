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
#include <QVBoxLayout>
#include "Interface/Areas/OutputArea.h"
#include "Interface/MainArea.h"
#include "Interface/Widgets/WindowMenuBar.h"
#include "Style/Style.h"

namespace Jam::Editor
{
    Application::Application(QWidget* parent) :
        QWidget(parent),
        _flags{NoFlag}
    {
        construct();
    }

    Application::~Application()
    {
        saveSettings();

        delete _menuBar;
        _menuBar = nullptr;

        delete _mainArea;
        _mainArea = nullptr;

        delete _layout;
        _layout = nullptr;
    }

    void Application::construct()
    {
        _layout = Style::verticalLayout();

        // Update the application window settings
        loadSettings();

        setWindowFlags(Qt::FramelessWindowHint);
        setMinimumSize(320, 480);

        Style::apply(this, ApplicationStyle);

        constructMenuBar();

        // Reload the last state of the working project.
        if (const QString& lastOpen = _cachedProjectPath;
            !lastOpen.isEmpty())
            loadProjectFromPath(lastOpen);

        if (!_mainArea)
        {
            _mainArea = new MainArea();
            setCentralWidget(_mainArea);
            newProject();
        }

        setLayout(_layout);
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

        _mainArea = new MainArea(string, this);
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

    void Application::setMenuBar(QWidget* widget) const
    {
        if (widget && _layout)
            _layout->insertWidget(0, widget);
    }

    void Application::setCentralWidget(QWidget* widget) const
    {
        if (widget && _layout)
            _layout->addWidget(widget, 1);
    }

}  // namespace Jam::Editor

// This is probably not a good idea to have these,
// fragmented like this. But, it separates methods
// by context, which makes it easier to manage.
//
// This will guarantee that if they are accidentally
// included elsewhere the compiler will issue an error.
#define APPLICATION_INLINE_DEFINITION 1

#include "Application.Dialogs.inl"
#include "Application.Events.inl"
#include "Application.Menu.inl"
#include "Application.Project.inl"
#include "Application.Settings.inl"
