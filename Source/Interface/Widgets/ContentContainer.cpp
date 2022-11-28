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
#include "Interface/Widgets/ContentContainer.h"
#include <QVBoxLayout>
#include "Interface/Extensions.h"

namespace Jam::Editor
{
    ContentContainer::ContentContainer(QWidget* parent) :
        QWidget(parent)
    {
        construct();
    }

    ContentContainer::~ContentContainer() = default;

    void ContentContainer::construct()
    {
        _layout = new QVBoxLayout();
        View::widgetDefaults(this);
        View::applyColorRoles(this, QPalette::Window);

        setLayout(_layout);
    }

    void ContentContainer::clear() const
    {
        if (_layout)
        {
            while (const auto item = _layout->takeAt(0))
            {
                delete item->widget();
                delete item;
            }
        }
    }

    void ContentContainer::setBorder(const int border) const
    {
        if (_layout)
            View::layoutDefaults(_layout, border);
    }

    void ContentContainer::setContentLayout(QLayout* layout) const
    {
        if (_layout)
        {
            clear();
            Q_ASSERT(_layout->count() == 0);

            _layout->addLayout(layout, 1);
        }
    }

    void ContentContainer::enterEvent(QEnterEvent* event)
    {
        QWidget::enterEvent(event);
        emit mouseEntered();
    }

    void ContentContainer::leaveEvent(QEvent* event)
    {
        QWidget::leaveEvent(event);
        emit mouseExited();
    }
}  // namespace Jam::Editor
