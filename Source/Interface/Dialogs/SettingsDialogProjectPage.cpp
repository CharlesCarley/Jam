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
#include "Interface/Dialogs/SettingsDialogProjectPage.h"
#include <QCheckBox>
#include <QVBoxLayout>
#include "Interface/Extensions.h"

namespace Jam::Editor
{
    SettingsDialogProjectPage::SettingsDialogProjectPage(
        SettingsDataState* state,
        QWidget*           parent) :
        QWidget(parent),
        _state(state)
    {
        Q_ASSERT(state);
        construct();
    }

    SettingsDialogProjectPage::~SettingsDialogProjectPage() = default;

    void SettingsDialogProjectPage::construct()
    {
        const auto projectLayout = new QVBoxLayout();
        View::layoutDefaults(projectLayout);

        const auto minify = new QCheckBox("Write compressed XML files");
        minify->setChecked(_state? _state->minify : false);

        const auto reloadLast = new QCheckBox("Reload last project on startup");
        reloadLast->setChecked(_state? _state->reload : false);

        projectLayout->addWidget(minify, 0, Qt::AlignTop);
        projectLayout->addWidget(reloadLast, 1, Qt::AlignTop);

        connect(minify, &QCheckBox::toggled, this, [=]
        {
            if (_state)
                _state->minify = !_state->minify;
        });

        connect(reloadLast, &QCheckBox::toggled, this, [=]
        {
            if (_state)
                _state->reload = !_state->reload;
        });

        setLayout(projectLayout);
    }

}  // namespace Jam::Editor
