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

#include "Interface/Dialogs/PaletteDialog.h"
#include <qtextedit.h>
#include <QLabel>
#include <QVBoxLayout>
#include "Interface/Style/Palette.h"
#include "Interface/Style/Style.h"

namespace Jam::Editor
{
    constexpr const char* BaseColorNames[]{
        ":Blue00",
        ":Blue01",
        ":Blue02",
        ":Blue03",
        ":Blue04",
        ":Green00",
        ":Green01",
        ":Green02",
        ":Green03",
        ":Green04",
        ":Red00",
        ":Red01",
        ":Red02",
        ":Red03",
        ":Red04",
        ":Yellow00",
        ":Yellow01",
        ":Yellow02",
        ":Yellow03",
        ":Yellow04",
    };

    constexpr const char* RolesNames[]{
        ":Shadow",
        ":Dark",
        ":Base",
        ":Mid",
        ":Midlight",
        ":Light",
        ":Highlight",
        ":Window",
        ":WindowText",
        ":Button",
        ":ButtonText",
        ":ToolTipBase",
        ":ToolTipText",
        ":Text",
        ":BrightText",
        ":HighlightedText",
        ":Link",
        ":LinkVisited",
        ":AlternateBase",
        ":PlaceholderText",
    };

    constexpr QPalette::ColorRole Roles[20] = {
        QPalette::Shadow,
        QPalette::Dark,
        QPalette::Base,
        QPalette::Mid,
        QPalette::Midlight,
        QPalette::Light,
        QPalette::Highlight,
        QPalette::Window,
        QPalette::WindowText,
        QPalette::Button,
        QPalette::ButtonText,
        QPalette::ToolTipBase,
        QPalette::ToolTipText,
        QPalette::Text,
        QPalette::BrightText,
        QPalette::HighlightedText,
        QPalette::Link,
        QPalette::LinkVisited,
        QPalette::AlternateBase,
        QPalette::PlaceholderText,
    };

    class ColorGrid final : public QWidget
    {
    public:
        ColorGrid();
    private:
        static QLayout* constructAccents();
        static QLayout* currentPalette();
    };

    ColorGrid::ColorGrid()
    {
        const auto hl = new QHBoxLayout();
        hl->addLayout(constructAccents(), 0);
        hl->addLayout(currentPalette(), 1);
        setLayout(hl);
    }

    QLayout* ColorGrid::constructAccents()
    {
        const auto layout = new QVBoxLayout();

        int     nr    = 0;
        QLabel* title = new QLabel();
        title->setText("Accents");
        layout->addWidget(title);

        QPalette pal;
        Palette::getAccentPalette(pal);
        for (const QPalette::ColorRole& role : Roles)
        {
            QHBoxLayout* hb = new QHBoxLayout();
            QWidget*     w  = new QWidget();
            w->setMaximumWidth(Style::hint(ButtonWidth));
            w->setMinimumWidth(Style::hint(ButtonWidth));
            w->setBackgroundRole(role);
            w->setAutoFillBackground(true);
            w->setPalette(pal);
            hb->addWidget(w);

            QLabel* l = new QLabel();
            l->setText(BaseColorNames[nr]);
            hb->addWidget(l, 1);
            layout->addLayout(hb, 1);
            ++nr;
        }
        return layout;
    }

    QLayout* ColorGrid::currentPalette()
    {
        const auto layout = new QVBoxLayout();
        QPalette   pal;
        Palette::getApplicationPalette(pal);

        QLabel* title = new QLabel();
        title->setText("Current");
        layout->addWidget(title);

        int nr = 0;
        for (const QPalette::ColorRole& role : Roles)
        {
            QHBoxLayout* hb = new QHBoxLayout();
            QWidget*     w  = new QWidget();

            w->setMaximumWidth(Style::hint(ButtonWidth));
            w->setMinimumWidth(Style::hint(ButtonWidth));
            w->setBackgroundRole(role);
            w->setAutoFillBackground(true);
            w->setPalette(pal);
            hb->addWidget(w);

            QLabel* l = new QLabel();
            l->setText(RolesNames[nr]);
            hb->addWidget(l, 1);

            layout->addLayout(hb, 1);
            ++nr;
        }
        return layout;
    }

    PaletteDialog::PaletteDialog(QWidget* parent) :
        Dialog("Palette", Close, parent)
    {
        construct();
    }

    PaletteDialog::~PaletteDialog() = default;

    void PaletteDialog::construct()
    {
        QVBoxLayout* layout = Style::verticalLayout();
        layout->addWidget(new ColorGrid(), 1);
        applyLayout(layout);
        setFixedSize(layout->sizeHint());
    }

}  // namespace Jam::Editor
