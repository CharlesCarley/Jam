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
#include "Interface/Areas/TodoArea.h"
#include <qtextformat.h>
#include <QLabel>
#include <QVBoxLayout>
#include "Interface/Area/Area.h"
#include "Interface/Area/AreaToolbar.h"
#include "Interface/Extensions.h"
#include "Interface/Style/Style.h"

namespace Jam::Editor
{
    TodoArea::TodoArea(AreaCreator* creator,
                       int32_t      type,
                       size_t       refId,
                       QWidget*     parent) :
        Area(creator, type, refId, parent),
        _todoType(type)
    {
        construct();
    }

    TodoArea::~TodoArea() = default;

    void TodoArea::construct()
    {
        Style::apply(this, AreaStyle);
        const auto layout = Style::verticalLayout();
        
        QLabel* fill = new QLabel(
            Su::join("Todo(", _todoType, "): What next?").c_str());

        QFont font = fill->font();
        font.setPointSize(18);
        fill->setFont(font);

        layout->addWidget(_toolbar);
        layout->addWidget(fill, 1, Qt::AlignCenter);

        setLayout(layout);
    }

}  // namespace Jam::Editor
