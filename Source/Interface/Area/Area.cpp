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
#include "Interface/Area/Area.h"
#include "Interface/Area/AreaToolbar.h"
#include "Interface/Constants.h"
#include "Interface/Events/BranchEvent.h"
#include "Interface/Extensions.h"

namespace Jam::Editor
{
    Area::Area(AreaCreator* creator,
               int32_t      type,
               QWidget*     parent) :
        QWidget(parent),
        _creator(creator),
        _type(type)
    {
        constructBase(type);
    }

    Area::~Area()
    {
        delete _toolbar;
        _toolbar = nullptr;
    }

    bool Area::propagate(QEvent* evt)
    {
        return event(evt);
    }

    // This Area's layout should be applied in the derived class's
    // construct method. It should layout this->_toolbar object with the
    // derived class's content layout, then append any specific content
    // to the toolbar in the derived Area.
    //
    // The default state of the toolbar is to allow a context menu.
    // The rest of it's definition is left up to the derived class.
    void Area::constructBase(const int32_t type)
    {
        View::widgetDefaults(this);
        View::applyColorRoles(this, Const::AreaRole);

        _toolbar = new AreaToolBar(_creator, type, this);

        connect(_toolbar,
                &AreaToolBar::wantsAreaSwitch,
                this,
                [=](const int swapTo)
                { emit wantsContextSwitch(swapTo); });
    }

}  // namespace Jam::Editor
