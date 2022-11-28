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

#include "Interface/Area/AreaBranch.h"
#include <QCoreApplication>
#include <QMenu>
#include <QSplitter>
#include <QVBoxLayout>
#include "Interface/Area/AreaBinaryLayout.h"
#include "Interface/Area/AreaNode.h"
#include "Interface/Areas/OutputArea.h"
#include "Interface/Events/MergeEvent.h"

namespace Jam::Editor
{
    AreaBranch::AreaBranch(const Qt::Orientation ori, AreaNode* left, AreaNode* right) :
        AreaContent(true),
        _left(left),
        _right(right),
        _orientation(ori)
    {
        construct();
    }

    AreaBranch::~AreaBranch()
    {
        delete _splitter;
        _splitter = nullptr;
    }

    void AreaBranch::construct()
    {
        _splitter = new AreaBinaryLayout(_orientation, _left, _right);

        connect(_splitter,
                &AreaBinaryLayout::wantsContextMenu,
                this,
                &AreaBranch::contextMenu);

        connect(_splitter,
                &AreaBinaryLayout::splitterMoved,
                this,
                &AreaBranch::splitterMoved);

        setLayout(_splitter);
    }

    AreaContent* AreaBranch::leftContent() const
    {
        if (_left)
            return _left->content();
        return nullptr;
    }

    AreaContent* AreaBranch::rightContent() const
    {
        if (_right)
            return _right->content();
        return nullptr;
    }

    qreal AreaBranch::ratio() const
    {
        if (_splitter)
            return _splitter->ratio();
        return 0.5;
    }

    void AreaBranch::setRatio(const qreal ratio) const
    {
        if (_splitter)
            _splitter->setRatio(ratio);
    }

    void AreaBranch::updateMask()
    {
        if (_left && _left->hasContent())
            _left->content()->updateMask();
        if (_right && _right->hasContent())
            _right->content()->updateMask();
    }

    bool AreaBranch::propagate(QEvent* evt)
    {
        if (_left && _left->hasContent())
            _left->content()->propagate(evt);
        if (_right && _right->hasContent())
            _right->content()->propagate(evt);
        return true;
    }

    void AreaBranch::dangleLayout() const
    {
        if (_splitter)
        {
            if (_splitter->disconnect(this))
                Log::writeLine("Disconnected signals");

            if (const QLayoutItem* lhs = _splitter->takeAt(0))
            {
                _left->setParent(nullptr);
                delete lhs;
            }

            if (const QLayoutItem* rhs = _splitter->takeAt(1))
            {
                _right->setParent(nullptr);
                delete rhs;
            }
        }
    }

    void AreaBranch::contextMenu(const QPoint& at) const
    {
        Log::writeLine("AreaPair => contextMenuEvent");
        QMenu context;

        const QString title1 = _orientation == Qt::Vertical ? "Join Top" : "Join Left";
        const QString title2 = _orientation == Qt::Vertical ? "Join Bottom" : "Join Right";

        AreaNode* parent = parentNode();

        context.addAction(
            title1, [=]
            { QCoreApplication::postEvent(parent, new MergeEvent(MergeLeftEvent)); });
        context.addAction(
            title2, [=]
            { QCoreApplication::postEvent(parent, new MergeEvent(MergeRightEvent)); });

        context.exec(at);
    }

    void AreaBranch::splitterMoved(const QPoint& at) const
    {
        if (_parent)
        {
            const QRect&  geom = geometry();
            const QPoint& gPos = mapToGlobal(geom.topLeft());

            QPointF dPos = at - QPointF(gPos.x(), gPos.y());

            if (geom.width() > 0)
                dPos.setX(dPos.x() / qreal(geom.width()));
            else
                dPos.setX(0);

            if (geom.height() > 0)
                dPos.setY(dPos.y() / qreal(geom.height()));
            else
                dPos.setY(0);

            _splitter->drag(dPos);
        }
    }

}  // namespace Jam::Editor
