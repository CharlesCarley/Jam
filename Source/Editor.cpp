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
#include "Editor.h"
#include <qpluginloader.h>
#include <QApplication>
#include "Interface/Application.h"
#include "Interface/Areas/OutputArea.h"
#include "Interface/Style/MainStyle.h"
#include "Interface/Widgets/IconButton.h"
#include "Math/Lg.h"
#include "State/App.h"
#include "Utils/Exception.h"
#include "Utils/ScopePtr.h"

using namespace Jam::Editor;

namespace Jam::Editor
{
    EditorApp::EditorApp(int& argc, char** argv, int i) :
        QApplication(argc, argv, i)
    {
        construct();
    }

    void EditorApp::construct() const
    {
        setWindowIcon(get(Icons::Cube));

        // Set up the style backend. Applies the MainStyle instance
        // to QApplication::setStyle()
        MainStyle::initialize();

        // State should only be valid during the
        // following scope
        State::App::initialize();

        // connect(this,
        //         &QApplication::aboutToQuit,
        //         this,
        //         &EditorApp::destruct);
    }

    void EditorApp::destruct()
    {
        State::App::finalize();
    }

}  // namespace Jam::Editor

int main(int argc, char* argv[])
{
    int returnCode;
    try
    {
        // for the default logic in qwindowsintegration.cpp:228 v6.2.4
        // Work around the fact that the return code from GetLastError() after a fail code
        // from SetProcessDpiAwarenessContext is not checking for compatibility first before
        // attempting to enable it.
        QCoreApplication::setAttribute(Qt::AA_PluginApplication);
        {
            auto appInstance = Jam::ScopePtr(new Jam::Editor::EditorApp(argc, argv));

            // Construct the application.
            auto appView = Jam::ScopePtr(new Application());

            // show it and run the main loop
            appView->post();

            returnCode = QApplication::exec();
        }

        // Destroy only after the application
        // has gone out of scope. This is intentional to
        // allow any state accessors to throw an exception
        // vs return null (App.cpp) 
        Jam::Editor::EditorApp::destruct();
    }
    catch (Jam::Exception& ex)
    {
        Jam::Console::writeLine(ex.what());
        // unused outside of flagging that an
        // error occurred(!=0).
        returnCode = 1;
    }
    return returnCode;
}
