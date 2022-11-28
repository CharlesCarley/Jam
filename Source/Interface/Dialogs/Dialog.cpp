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

#include "Interface/Dialogs/Dialog.h"
#include <qevent.h>
#include <QFileDialog>
#include <QVBoxLayout>
#include "Interface/Extensions.h"
#include "Interface/Widgets/ContentContainer.h"
#include "Interface/Widgets/OkCancelWidget.h"
#include "Interface/Widgets/WindowTitlebar.h"

class QVBoxLayout;
class QTreeWidgetItem;
class QHBoxLayout;
class QTreeWidget;
class QLineEdit;

namespace Jam::Editor
{
    Dialog::Dialog(const int options, QWidget* parent) :
        QDialog(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
    {
        constructDialog(options & ~Minimize);
    }

    Dialog::Dialog(QString title, const int options, QWidget* parent) :
        QDialog(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint),
        _title(std::move(title))
    {
        constructDialog(options & ~Minimize);
    }

    Dialog::~Dialog() = default;

    void Dialog::constructDialog(const int options)
    {
        View::dialogDefaults(this);
        setModal(true);

        // construct the surrounding widgets for the frame
        _layout = new QVBoxLayout();
        View::layoutDefaults(_layout, _border << 1);

        _content = new QVBoxLayout();
        _center  = new ContentContainer();
        _center->setBorder(_border << 1);

        WindowTitlebar* title = new WindowTitlebar(_title, options);
        OkCancelWidget* apply = nullptr;
        if (options & OkCancel)
            apply = new OkCancelWidget(Qt::AlignRight, this);

        _content->addWidget(title, 0);
        _content->addWidget(_center, 1);
        if (apply)
            _content->addWidget(apply, 0);

        bindSignals(options, title, apply);

        ContentContainer* cent = new ContentContainer();
        cent->setContentLayout(_content);
        cent->setBorder(1);
        View::applyColorRoles(cent, _borderRole);

        connect(cent, &ContentContainer::mouseEntered, this, &Dialog::resetCursor);
        setMouseTracking(true);
        _layout->addWidget(cent, 1);

        View::applyColorRoles(this, _borderRole);
        setLayout(_layout);
    }

    void Dialog::bindSignals(
        const int             opts,
        const WindowTitlebar* title,
        const OkCancelWidget* applyAndExitOrExit)
    {
        if (applyAndExitOrExit)
        {
            connect(applyAndExitOrExit,
                    &OkCancelWidget::okClicked,
                    this,
                    [=]
                    { emit okClicked(); });

            connect(applyAndExitOrExit,
                    &OkCancelWidget::cancelClicked,
                    this,
                    [=]
                    { emit cancelClicked(); });
        }

        if (opts & Close)
            connect(title, &WindowTitlebar::exit, this, [=]
                    { close(); });
        if (opts & Maximize)
            connect(title, &WindowTitlebar::maximize, this, &Dialog::maximize);
        if (!(opts & Locked))
            connect(title, &WindowTitlebar::dragDialog, this, &Dialog::drag);
    }

    void Dialog::applyLayout(QLayout* layout)
    {
        if (layout)
        {
            if (_center)
            {
                _center->setContentLayout(layout);
                _center->setBorder(_border);
                View::applyColorRoles(this, _borderRole);
            }
        }
    }

    void Dialog::setBorder(int border)
    {
        if (_layout)
        {
            _border = border;
            View::layoutDefaults(_layout, _border);
            View::applyColorRoles(this, _borderRole);
        }
    }

    void Dialog::maximize() const
    {
        if (QWidget* winWidget = window())
        {
            if (winWidget->windowState() != Qt::WindowMaximized)
            {
                View::layoutDefaults(_layout, 0);
                winWidget->setWindowState(Qt::WindowMaximized);
            }
            else
            {
                View::layoutDefaults(_layout, _border);
                winWidget->setWindowState(Qt::WindowNoState);
            }
        }
    }

    void Dialog::mousePressEvent(QMouseEvent* event)
    {
        if (!isMaximized())
        {
            if (event->button() == Qt::LeftButton && _scaleEdge != None)
            {
                _last = event->globalPosition();
                _drag = true;
            }
        }
    }

    void Dialog::mouseReleaseEvent(QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton)
            clearDrag();
    }

    void Dialog::mouseMoveEvent(QMouseEvent* event)
    {
        if (_drag)
        {
            const QPointF mco = event->globalPosition();
            const QPoint  xy  = mco.toPoint();

            const int x0 = xy.x();
            const int y0 = xy.y();

            if (QWidget* win = window())
            {
                int   x1, y1, x2, y2;
                QRect geom = win->geometry();
                geom.getCoords(&x1, &y1, &x2, &y2);

                switch (_scaleEdge)
                {
                case LeftTop:
                    geom.adjust(x0 - x1, y0 - y1, 0, 0);
                    win->setGeometry(geom);
                    break;
                case RightTop:
                    geom.adjust(0, y0 - y1, x0 - x2, 0);
                    win->setGeometry(geom);
                    break;
                case RightBottom:
                    geom.adjust(0, 0, x0 - x2, y0 - y2);
                    win->setGeometry(geom);
                    break;
                case LeftBottom:
                    geom.adjust(x0 - x1, 0, 0, y0 - y2);
                    win->setGeometry(geom);
                    break;
                case Left:
                    geom.adjust(x0 - x1, 0, 0, 0);
                    win->setGeometry(geom);
                    break;
                case Right:
                    geom.adjust(0, 0, x0 - x2, 0);
                    win->setGeometry(geom);
                    break;
                case Top:
                    geom.adjust(0, y0 - y1, 0, 0);
                    win->setGeometry(geom);
                    break;
                case Bottom:
                    geom.adjust(0, 0, 0, y0 - y2);
                    win->setGeometry(geom);
                    break;
                case None:
                default:
                    break;
                }
            }
        }
        else
            computeCursor(event->globalPosition());
    }

    void Dialog::enterEvent(QEnterEvent* event)
    {
        if (!_drag)
            computeCursor(event->globalPosition());
    }

    void Dialog::leaveEvent(QEvent* event)
    {
        if (!_drag)
            setCursor(Qt::ArrowCursor);
    }

    void Dialog::drag(const QPointF& offset) const
    {
        if (QWidget* winWidget = window())
            winWidget->move(int(offset.x()), int(offset.y()));
    }

    void Dialog::clearDrag()
    {
        _last      = {};
        _drag      = false;
        _scaleEdge = None;
    }

    void Dialog::computeCursor(const QPointF& offset)
    {
        _scaleEdge = computeEdge(offset);
        switch (_scaleEdge)
        {
        case RightBottom:
        case LeftTop:
            setCursor(Qt::CursorShape::SizeFDiagCursor);
            break;
        case LeftBottom:
        case RightTop:
            setCursor(Qt::CursorShape::SizeBDiagCursor);
            break;
        case Right:
        case Left:
            setCursor(Qt::CursorShape::SizeHorCursor);
            break;
        case Bottom:
        case Top:
            setCursor(Qt::CursorShape::SizeVerCursor);
            break;
        case None:
        default:
            setCursor(Qt::ArrowCursor);
        }
    }

    void Dialog::resetCursor()
    {
        if (!_drag)
        {
            setCursor(Qt::ArrowCursor);
            clearDrag();
        }
    }

    Dialog::ScaleEdge Dialog::computeEdge(const QPointF& offset) const
    {
        if (_border <= 0)
            return None;

        QRect geom;
        View::windowRect(geom, this);

        int x1, y1, x2, y2;
        geom.getCoords(&x1, &y1, &x2, &y2);

        const auto loc = offset;
        const int  x0  = int(loc.x());
        const int  y0  = int(loc.y());

        const int cornerTest = 12 * _border;
        const int edgeTest   = 2 * _border;

        // corner tests take precedence
        if (QRect{x1, y1, cornerTest, cornerTest}.contains(x0, y0))
            return LeftTop;
        if (QRect{x2, y2, cornerTest, cornerTest}.contains(x0, y0))
            return RightBottom;
        if (QRect{x1, y2, cornerTest, cornerTest}.contains(x0, y0))
            return LeftBottom;
        if (QRect{x2, y1, cornerTest, cornerTest}.contains(x0, y0))
            return RightTop;

        if (QRect{x1, y1, edgeTest, (y2 - y1)}.contains(x0, y0))
            return Left;
        if (QRect{x2, y1, edgeTest, (y2 - y1)}.contains(x0, y0))
            return Right;
        if (QRect{x1, y1, x2 - x1, edgeTest}.contains(x0, y0))
            return Top;
        if (QRect{x1, y2, x2 - x1, edgeTest}.contains(x0, y0))
            return Bottom;
        return None;
    }

}  // namespace Jam::Editor
