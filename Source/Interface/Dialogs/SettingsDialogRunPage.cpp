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
#include "Interface/Dialogs/SettingsDialogRunPage.h"
#include <QCheckBox>
#include <QVBoxLayout>
#include "Interface/Extensions.h"

namespace Jam::Editor
{
    SettingsDialogRunPage::SettingsDialogRunPage(
        SettingsDataState* state,
        QWidget*           parent) :
        QWidget(parent),
        _state(state)
    {
        construct();
    }

    SettingsDialogRunPage::~SettingsDialogRunPage() = default;

    void SettingsDialogRunPage::construct()
    {
        const auto layout = new QVBoxLayout();
        View::layoutDefaults(layout);

        const auto clearOnRun = new QCheckBox("Clear output on run");
        clearOnRun->setChecked(_state ? _state->clearOnRun : false);

        layout->addWidget(clearOnRun, 1, Qt::AlignTop);

        connect(clearOnRun, &QCheckBox::toggled, this, [=]
                {
                    if (_state)
                        _state->clearOnRun = !_state->clearOnRun;
                });

        setLayout(layout);
    }

}  // namespace Jam::Editor
