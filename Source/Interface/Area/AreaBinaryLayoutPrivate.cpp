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
#include "Interface/Area/AreaBinaryLayoutPrivate.h"
#include <QApplication>
#include <QMouseEvent>
#include <QScreen>
#include "Interface/Area/AreaBinaryLayout.h"
#include "Interface/Constants.h"
#include "Interface/Extensions.h"

namespace Jam::Editor
{
    AreaBinaryLayoutPrivate::AreaBinaryLayoutPrivate(AreaBinaryLayout* par) :
        _parent(par)
    {
        View::widgetDefaults(this);
        View::applyColorRoles(this, Const::SplitterRole);
    }

    QSize AreaBinaryLayoutPrivate::appSize()
    {
        if (_root == nullptr)
            _root = QApplication::activeWindow();

        if (_root)
        {
            if (const QScreen* sc = _root->screen())
            {
                const QPoint loc = _root->pos();
                QSize        scl = _root->size();

                QSize calc = sc->size();
                calc -= scl;
                calc -= QSize(loc.x(), loc.y());

                if (calc.height() > 0 && calc.height() < 100)
                    scl.setHeight(scl.height() + calc.height());
                return scl;
            }
            return _root->size();
        }
        return {};
    }

    void AreaBinaryLayoutPrivate::mousePressEvent(QMouseEvent* event)
    {
        if (event != nullptr)
        {
            if (event->button() == Qt::LeftButton)
                grabMouse();
        }
    }

    void AreaBinaryLayoutPrivate::mouseReleaseEvent(QMouseEvent* event)
    {
        if (event != nullptr)
        {
            if (event->button() == Qt::LeftButton)
                releaseMouse();
        }
    }

    void AreaBinaryLayoutPrivate::mouseMoveEvent(QMouseEvent* event)
    {
        if (_parent)
        {
            const auto l = event->globalPosition();

            emit _parent->splitterMoved(QPoint{
                (int)l.x(),
                (int)l.y(),
            });
        }
    }

    void AreaBinaryLayoutPrivate::enterEvent(QEnterEvent* event)
    {
        if (_parent)
            setCursor(_parent->orientation() == Qt::Horizontal ? Qt::SplitHCursor : Qt::SplitVCursor);
    }

    void AreaBinaryLayoutPrivate::leaveEvent(QEvent* event)
    {
        setCursor(Qt::ArrowCursor);
    }

    void AreaBinaryLayoutPrivate::contextMenuEvent(QContextMenuEvent* event)
    {
        if (_parent)
            emit _parent->wantsContextMenu(event->globalPos());
    }

}  // namespace Jam::Editor
