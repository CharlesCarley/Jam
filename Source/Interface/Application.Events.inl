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
// ReSharper disable CppNonInlineFunctionDefinitionInHeaderFile
#pragma once
#include <QApplication>
#include "Interface/Application.h"
#include "Interface/Events/EventTypes.h"
#include "Interface/MainArea.h"

namespace Jam::Editor
{
    void Application::notifyProjectOpened() const
    {
        if (_mainArea)
            QApplication::postEvent(_mainArea, new QEvent((QEvent::Type)ProjectOpened));
    }

    void Application::notifyProjectClosed() const
    {
        if (_mainArea)
            QApplication::postEvent(_mainArea, new QEvent((QEvent::Type)ProjectClosed));
    }

    void Application::notifyProjectStateChanged() const
    {
        if (_mainArea)
            QApplication::postEvent(_mainArea, new QEvent((QEvent::Type)ProjectChanged));
    }

    

    bool Application::event(QEvent* event)
    {
        switch ((int)event->type())
        {
        case LayerSelect:
            _mainArea->notify(event);
            return false;
        case ProjectClosed:
        case ProjectOpened:
        default:
            break;
        }
        return QMainWindow::event(event);
    }

}  // namespace Jam::Editor
