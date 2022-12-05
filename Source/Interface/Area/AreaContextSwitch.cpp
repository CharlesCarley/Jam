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
#include "Interface/Area/AreaContextSwitch.h"
#include <QBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QMouseEvent>
#include <QPushButton>
#include "AreaCreator.h"
#include "Interface/Extensions.h"
#include "Interface/Style/Style.h"

namespace Jam::Editor
{
    AreaContextSwitch::AreaContextSwitch(
        AreaCreator*  creator,
        const int32_t type,
        QWidget*      parent) :
        QWidget(parent),
        _creator(creator),
        _type(type)
    {
        construct();
    }

    void AreaContextSwitch::construct()
    {
        Style::apply(this, TransparentStyle);
        const auto layout = Style::horizontalLayout();

        QPushButton* button = Style::toolButton(
            _creator->iconFromType(_type));

        connect(button,
                &QPushButton::clicked,
                this,
                &AreaContextSwitch::onMousePressEvent);

        QLabel* title = new QLabel(_creator->nameFromType(_type));
        Style::apply(title, AreaToolLabelStyle);

        layout->addWidget(button);
        layout->addWidget(title);

        setLayout(layout);
    }

    void AreaContextSwitch::onMousePressEvent()
    {
        QMenu menu(this);
        Style::apply(&menu, AreaToolMenuItemStyle);

        const int32_t nr = _creator->getNumberOfTypes();
        for (int32_t i = 0; i < nr; ++i)
        {
            QString name = _creator->nameFromType(i);

            menu.addAction(get(_creator->iconFromType(i)), name, [=]
                           { emit wantsContextSwitch(i); });
        }

        setFocus(Qt::FocusReason::PopupFocusReason);
        menu.exec(mapToGlobal(pos()));
    }

}  // namespace Jam::Editor
