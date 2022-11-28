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

#include "Interface/Dialogs/AboutDialog.h"
#include <qtextedit.h>
#include <QVBoxLayout>

namespace Jam::Editor
{
    constexpr const char* AboutText =
        "# Jam Engine\n"
        "\n"
        "https://github.com/CharlesCarley/JamEngine"
        "\n\n"
        "Copyright (c) Charles Carley.\n"
        "\n";

    AboutDialog::AboutDialog(QWidget* parent) :
        Dialog("About", Close, parent)
    {
        construct();
    }

    AboutDialog::~AboutDialog() = default;

    void AboutDialog::construct()
    {
        QVBoxLayout* layout = new QVBoxLayout();
        setFixedSize(640, 480);

        QTextEdit* text = new QTextEdit();
        text->setMarkdown(AboutText);
        text->setReadOnly(true);
        layout->addWidget(text, 1);
        applyLayout(layout);
    }

}  // namespace Jam::Editor
