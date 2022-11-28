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

#include "StackedPanel.h"
#include "Interface/Areas/OutputArea.h"
#include "Interface/Extensions.h"

namespace Jam::Editor
{
    StackedPanel::StackedPanel(QWidget* parent) :
        QWidget(parent)
    {
        construct();
    }

    StackedPanel::~StackedPanel()
    {
        delete _layout;
        _layout = nullptr;
    }

    void StackedPanel::addWidget(
        QWidget*  widget,
        const int expand) const
    {
        _contentLayout->addWidget(widget, expand);
    }

    void StackedPanel::addLayout(QLayout* widget, int expand) const
    {
        _contentLayout->addLayout(widget, expand);
    }

    void StackedPanel::setLabel(const QString& label) const
    {
        _title->setText(label);
    }

    QSize StackedPanel::sizeHint() const
    {
        return _layout->geometry().size();
    }

    void StackedPanel::construct()
    {
        View::applyColorRoles(this);

        _contentLayout = new QVBoxLayout();
        _layout        = new QVBoxLayout();

        View::layoutDefaults(_contentLayout,
                             StackedPanelContentMargin,
                             StackedPanelContentSpacing);

        View::layoutDefaults(_layout,
                             StackedPanelMargin,
                             StackedPanelContentSpacing >> 1);

        // +--------------------------------------------+
        // |(V|>) .. Title |     click/drag             |
        // |--------------------------------------------|
        // |   Content Area                             |
        // +--------------------------------------------+

        QWidget*     header       = new QWidget();
        QHBoxLayout* headerLayout = new QHBoxLayout();
        View::layoutDefaults(headerLayout, 0, 1);
        View::applyColorRoles(header);

        // use setLabel, so blank initially...
        _title = new QLabel("");

        QFont font = _title->font();
        font.setPointSize(12);

        _title->setFont(font);
        View::applyColorRoles(_title, QPalette::NoRole, QPalette::Text);

        headerLayout->addWidget(_title);
        header->setLayout(headerLayout);

        _layout->addWidget(header, 0, Qt::AlignTop);
        _layout->addLayout(_contentLayout, 1);

        setLayout(_layout);
    }

}  // namespace Jam::Editor
