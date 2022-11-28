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

#include "Interface/Widgets/OkCancelWidget.h"
#include <QBoxLayout>
#include <QPushButton>
#include "Interface/Constants.h"
#include "Interface/Extensions.h"

namespace Jam::Editor
{
    OkCancelWidget::OkCancelWidget(QWidget* parent) :
        QWidget(parent)
    {
        View::widgetDefaults(this);
        if (parent)
            View::copyColorRoles(this, parent);
        else
            View::emptyWidget(this);

        construct(Qt::AlignRight);
    }

    OkCancelWidget::OkCancelWidget(const Qt::Alignment& align, QWidget* parent) :
        QWidget(parent)
    {
        View::widgetDefaults(this);
        if (parent)
            View::copyColorRoles(this, parent);
        else
            View::emptyWidget(this);
        construct(align);
    }

    OkCancelWidget::~OkCancelWidget() = default;

    void OkCancelWidget::construct(const Qt::Alignment& align)
    {
        _layout = new QHBoxLayout();
        View::layoutDefaults(_layout, 6);

        _layout->setSpacing(Const::SplitterSize);

        QPushButton* okay = new QPushButton("Ok");
        View::pushButtonDefaults(okay);
        QPushButton* skip = new QPushButton("Cancel");
        View::pushButtonDefaults(skip);

        if (align == Qt::AlignRight)
            _layout->addStretch(1);

        _layout->addWidget(okay);
        _layout->addWidget(skip);

        if (align != Qt::AlignRight)
            _layout->addStretch(1);

        connect(okay, &QPushButton::clicked, this, [=]
                { emit okClicked(); });
        connect(skip, &QPushButton::clicked, this, [=]
                { emit cancelClicked(); });

        View::applyColorRoles(this, QPalette::Window);
        setLayout(_layout);
    }
}  // namespace Jam::Editor
