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
#include "State/App.h"
#include "FrameStackManager.h"
#include "State/OutputLogMonitor.h"
#include "State/ProjectManager.h"

namespace Jam::Editor::State
{
    App* App::_instance = nullptr;
    App::App()          = default;

    App::~App()
    {
        delete _project;
        _project = nullptr;

        delete _layerStack;
        _layerStack = nullptr;

        delete _output;
        _output = nullptr;
    }

    void App::initialize()
    {
        if (_instance)
            throw Exception("The application state is already initialized.");

        _instance = new App();
        _instance->setup();
    }

    void App::setup()
    {
        _output     = new OutputLogMonitor();
        _layerStack = new FrameStackManager();
        _project    = new ProjectManager();
    }

    void App::finalize()
    {
        delete _instance;
        _instance = nullptr;
    }

    bool App::isValid()
    {
        return _instance != nullptr;
    }

    App& App::instance()
    {
        if (!_instance)
            throw Exception("Invalid application instance.");
        return *_instance;
    }

    ProjectManager* App::projectState() const
    {
        if (!_project)
            throw Exception("Invalid project instance.");
        return _project;
    }

    OutputLogMonitor* App::outputState() const
    {
        if (!_output)
            throw Exception("Invalid output instance.");
        return _output;
    }

    FrameStackManager* App::layerStack() const
    {
        if (!_layerStack)
            throw Exception("Invalid stack instance.");
        return _layerStack;
    }

    ProjectManager* projectState()
    {
        if (App::isValid())
            return App::instance().projectState();
        throw Exception("Invalid application state data.");
    }

    OutputLogMonitor* outputState()
    {
        if (App::isValid())
            return App::instance().outputState();
        throw Exception("Invalid application state data.");
    }

    FrameStack* frameStack()
    {
        if (const FrameStackManager* proj = layerStack())
            return proj->stack();
        throw Exception("Invalid application state data.");
    }

    FrameStackManager* layerStack()
    {
        if (App::isValid())
            return App::instance().layerStack();
        throw Exception("Invalid application state data.");
    }

    GridLayer* gridLayer()
    {
        return layerStack()->cast<GridLayer, GridType>(0);
    }

    FunctionLayer* functionLayer()
    {
        return layerStack()->cast<FunctionLayer, FunctionType>(1);
    }

}  // namespace Jam::Editor::State
