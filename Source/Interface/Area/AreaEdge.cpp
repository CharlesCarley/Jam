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

#include "Interface/Area/AreaEdge.h"
#include <QLabel>
#include <QMenu>
#include <QMouseEvent>
#include "Interface/Constants.h"
#include "Interface/Extensions.h"

namespace Jam::Editor
{
    AreaEdge::AreaEdge(const Qt::Orientation ori, QWidget* parent) :
        QWidget(parent),
        _orientation(ori)
    {
        View::widgetDefaults(this);
        View::applyColorRoles(this, Const::SplitterRole, QPalette::NoRole);
    }

    void AreaEdge::contextMenuEvent(QContextMenuEvent* event)
    {
        emit wantsContextMenu(_orientation, event->globalPos());
    }

    void AreaEdge::enterEvent(QEnterEvent* event)
    {
        setCursor(_orientation == Qt::Horizontal ? Qt::SplitVCursor : Qt::SplitHCursor);
    }

    void AreaEdge::leaveEvent(QEvent* event)
    {
        setCursor(Qt::ArrowCursor);
    }

    QSize AreaEdge::sizeHint() const
    {
        if (_orientation == Qt::Vertical)
            return {Const::SplitterSize, -1};
        return {-1, Const::SplitterSize};
    }

}  // namespace Jam::Editor
