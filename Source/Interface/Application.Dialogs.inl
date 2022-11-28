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
#include "Interface/Dialogs/AboutDialog.h"
#include "Interface/Dialogs/SettingsDialog.h"

namespace Jam::Editor
{
    void Application::viewDocs() const
    {
        Log::writeLine("TODO: view documentation");
    }

    void Application::showAbout()
    {
        Log::writeLine("Launching about");

        AboutDialog dia(this);
        dia.exec();
    }

    void Application::editSettings()
    {
        Log::writeLine("Launching settings");

        SettingsDialog settings(this);
        settings.selectOption(SettingsDialog::SettingRoot);
        settings.exec();
    }

}  // namespace Jam::Editor
