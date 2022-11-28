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
#pragma once
#include <QLayout>

class QSplitter;

namespace Jam::Editor
{
    class AreaBranch;
    class AreaNode;

    class AreaBinaryLayout final : public QLayout
    {
        Q_OBJECT
    signals:
        void wantsContextMenu(const QPoint& at);

        void splitterMoved(const QPoint& xy);

    private:
        friend class AreaBinaryLayoutPrivate;

        Qt::Orientation     _orientation{Qt::Horizontal};
        qreal               _split{0.5};
        QList<QLayoutItem*> _items;

        void construct(AreaNode* left, AreaNode* right);

    public:
        AreaBinaryLayout(const Qt::Orientation& orientation, AreaNode* left, AreaNode* right);
        ~AreaBinaryLayout() override;

        const Qt::Orientation& orientation() const;

        QSize sizeHint() const override;

        void addItem(QLayoutItem*) override;

        QLayoutItem* itemAt(int index) const override;

        QLayoutItem* takeAt(int index) override;

        int count() const override;

        void setGeometry(const QRect&) override;

        qreal ratio() const;

        void setRatio(qreal ratio);

        void drag(const QPointF& loc);
    };

    inline qreal AreaBinaryLayout::ratio() const
    {
        return _split;
    }

}  // namespace Jam::Editor
