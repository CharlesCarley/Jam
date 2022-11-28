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
#include "Interface/Dialogs/SettingsDialogRootPage.h"
#include <QLabel>
#include <QVBoxLayout>
#include "Interface/Widgets/IconButton.h"

namespace Jam::Editor
{
    SettingsDialogRootPage::SettingsDialogRootPage(
        SettingsDataState* state,
        QWidget*           parent) :
        QWidget(parent),
        _state(state)
    {
        construct();
    }

    SettingsDialogRootPage::~SettingsDialogRootPage() = default;

    void SettingsDialogRootPage::construct()
    {
        const auto layout = new QHBoxLayout();

        const auto brush = IconButton::createTitleButton(Icons::Clear, this);
        layout->addWidget(brush, 0, Qt::AlignTop);

        connect(brush, &QPushButton::clicked, this, [=]
        {
                if (_state)
                {
                    *_state = SettingsDataState();
                    PersistentSettings().apply(*_state);
                }
        });

        const auto label = new QLabel("Clear settings (needs restart)");
        layout->addWidget(label, 1, Qt::AlignTop);
        setLayout(layout);
    }

}  // namespace Jam::Editor
