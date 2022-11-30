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
#include "FrameStack/FunctionLayer.h"
#include "FrameStack/GridLayer.h"

namespace Jam
{
    class FrameStack;
}

namespace Jam::Editor::State
{
    class FrameStackManager;
    class OutputLogMonitor;
    class ProjectManager;

    class App
    {
    private:
        static App* _instance;

        // TODO: place pointers to specific state classes here.

        FrameStackManager* _layerStack{nullptr};
        ProjectManager*    _project{nullptr};
        OutputLogMonitor*  _output{nullptr};

    private:
        App();
        ~App();

        void setup();

    public:
        // Singleton access to the application state.
        static App& instance();
        static void initialize();
        static void finalize();
        static bool isValid();

        ProjectManager* projectState() const;

        OutputLogMonitor* outputState() const;

        FrameStackManager* layerStack() const;
    };

    // Public api for state access.
    //
    // (Favor access this way vs ApplicationState::instance()->...)
    // The assumption is it's either valid or an exception.

    extern ProjectManager* projectState();

    extern OutputLogMonitor* outputState();

    extern FrameStackManager* layerStack();

    extern GridLayer* gridLayer();

    extern FunctionLayer* functionLayer();

}  // namespace Jam::Editor::State
