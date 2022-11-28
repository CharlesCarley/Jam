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
#include "Interface/Area/AreaToolbar.h"
#include <QLabel>
#include "AreaCreator.h"
#include "Interface/Area/AreaContextSwitch.h"
#include "Interface/Constants.h"
#include "Interface/Extensions.h"

namespace Jam::Editor
{
    AreaToolBar::AreaToolBar(AreaCreator* creator, int32_t type, QWidget* parent) :
        QWidget(parent),
        _type(type),
        _creator(creator)
    {
        construct();
    }

    AreaToolBar::~AreaToolBar()
    {
        if (_toolbar)
        {
            while (const QLayoutItem* item = _toolbar->takeAt(0))
            {
                delete item->widget();
                delete item;
            }
        }
        delete _toolbar;
        _toolbar = nullptr;
    }

    void AreaToolBar::addWidget(QWidget* widget, int stretch, Qt::Alignment al) const
    {
        if (_toolbar && widget)
            _toolbar->addWidget(widget, stretch, al);
    }

    void AreaToolBar::addStretch() const
    {
        if (_toolbar)
            _toolbar->addStretch();
        else
            qWarning("called without a valid toolbar");
    }

    void AreaToolBar::construct()
    {
        View::widgetDefaults(this);
        View::applyColorRoles(this, Const::ToolRole);

        _toolbar = new QHBoxLayout();
        View::layoutDefaults(_toolbar);

        const auto sw = new AreaContextSwitch(_creator,
                                              _type,
                                              this);
        _toolbar->addWidget(sw, 0, Qt::AlignLeft);
        _toolbar->addWidget(new QLabel(_creator->nameFromType(_type)), 1, Qt::AlignLeft);

        connect(sw,
                &AreaContextSwitch::wantsContextSwitch,
                this,
                [=](const int switchTo)
                { emit wantsAreaSwitch(switchTo); });

        setLayout(_toolbar);
    }

}  // namespace Jam::Editor
