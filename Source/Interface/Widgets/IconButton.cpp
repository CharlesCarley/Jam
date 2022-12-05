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
#include "Interface/Widgets/IconButton.h"
#include <QPushButton>
#include "Interface/Constants.h"
#include "Interface/Extensions.h"
#include "Interface/Style/Palette.h"

namespace Jam::Editor
{
    IconButton::IconButton(QWidget* parent) :
        QPushButton(parent)
    {
    }

    IconButton* IconButton::createButton(const Icons::Icon& icon, QWidget* parent)
    {
        IconButton* button = new IconButton(parent);

        Style::apply(button, TransparentStyle);

        // force the size of the icon
        button->setMaximumSize({24, 24});

        button->setFlat(true);
        button->setIcon(get(icon));
        return button;
    }

    IconButton* IconButton::createToolButton(const Icons::Icon& icon, QWidget* parent)
    {
        return Buttons::createAreaToolButton(icon, parent);
    }

    IconButton* IconButton::createTitleButton(const Icons::Icon& icon, QWidget* parent)
    {
        return Buttons::createTitleButton(icon, parent);
    }

    IconButton* IconButton::create(const Icons::Icon& icon, QWidget* parent)
    {
        IconButton* button = createButton(icon, parent);
        button->setProperty("iconType", 3);
        return button;
    }

    QCheckBox* Buttons::createCheckBox(const QString& label, bool initialState)
    {
        const auto cb = new QCheckBox(label);
        Style::apply(cb, QCheckBoxStyle);
        cb->setChecked(initialState);
        return cb;
    }

    IconButton* Buttons::createTitleButton(const Icons::Icon& icon, QWidget* parent)
    {
        IconButton* button = IconButton::create(icon, parent);
        button->setProperty("iconType", 2);
        //Palette::swapColorRole(button, QPalette::AlternateBase, Styles::MenuBarIconAccent);
        return button;
    }
    

    IconButton* Buttons::createAreaToolButton(const Icons::Icon& icon, QWidget* parent)
    {
        IconButton* button = IconButton::create(icon, parent);
        button->setProperty("iconType", 1);
        //Palette::swapAccentRole(button, QPalette::AlternateBase, Styles::AreaIconAccent);
        return button;
    }

}  // namespace Jam::Editor
