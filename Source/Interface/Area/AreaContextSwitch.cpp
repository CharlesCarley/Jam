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
#include <QMenu>
#include <QMouseEvent>
#include "AreaCreator.h"
#include "Interface/Extensions.h"

namespace Jam::Editor
{
    AreaContextSwitch::AreaContextSwitch(AreaCreator* creator,
                                         int32_t      type,
                                         QWidget*     parent) :
        QPushButton(parent),
        _creator(creator),
        _type(type)

    {
        construct();
    }

    void AreaContextSwitch::construct()
    {
        setIcon(_creator->iconFromType(_type));
        View::emptyWidget(this);
    }

    void AreaContextSwitch::mousePressEvent(QMouseEvent* e)
    {
        QMenu menu(this);

        const int32_t nr = _creator->getNumberOfTypes();
        for (int32_t i = 0; i < nr; ++i)
        {
            QString name = _creator->nameFromType(i);

            menu.addAction(_creator->iconFromType(i), name, [=]
                           { emit wantsContextSwitch(i); });
        }

        setFocus(Qt::FocusReason::PopupFocusReason);
        menu.exec(mapToGlobal(pos()));
    }

}  // namespace Jam::Editor
