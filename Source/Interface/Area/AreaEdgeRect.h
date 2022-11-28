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
#include <QWidget>

class QHBoxLayout;
class QVBoxLayout;

namespace Jam::Editor
{
    class AreaContent;
    class AreaNode;
    class Area;
    class AreaEdge;

    enum AreaEdgeVisibility
    {
        AreaEdgeNone    = 0x00,
        AreaEdgeLeft    = 0x01,
        AreaEdgeTop     = 0x02,
        AreaEdgeRight   = 0x04,
        AreaEdgeBottom  = 0x08,
        AreaEdgeAll     = AreaEdgeLeft | AreaEdgeTop | AreaEdgeRight | AreaEdgeBottom,
        AreaEdgeHTop    = AreaEdgeLeft | AreaEdgeTop | AreaEdgeRight,
        AreaEdgeHBottom = AreaEdgeLeft | AreaEdgeBottom | AreaEdgeRight,
        AreaEdgeVLeft   = AreaEdgeLeft | AreaEdgeTop | AreaEdgeBottom,
        AreaEdgeVRight  = AreaEdgeRight | AreaEdgeTop | AreaEdgeBottom,
    };

    class AreaEdgeRect final : public QWidget
    {
        Q_OBJECT
    public slots:
        void contextMenu(Qt::Orientation ori, const QPoint& at);
    signals:
        void wantsContextMenu(Qt::Orientation from,
                              Qt::Orientation to,
                              const QPoint&   at);

    private:
        QVBoxLayout* _vLayout{nullptr};
        QHBoxLayout* _hLayout{nullptr};
        AreaEdge*    _top{nullptr};
        AreaEdge*    _bottom{nullptr};
        AreaEdge*    _left{nullptr};
        AreaEdge*    _right{nullptr};
        int          _mask{AreaEdgeAll};

        void construct(Area* root);

        void bindContextMenu(const AreaEdge* splitter);

        static void applyEdgeVisibility(AreaEdge* edge, bool show);

    public:
        explicit AreaEdgeRect(Area* root);
        ~AreaEdgeRect() override;

        QVBoxLayout* container() const;

        void updateMask(int mask);

        static int computeMask(const AreaNode* branch,
                               const AreaNode* current,
                               int             mask,
                               const int       values[4]);
        static int computeMask(const AreaNode*    branch,
                               const AreaContent* current,
                               int                mask,
                               const int          values[4]);

        static int computeMask(const AreaNode* parent,
                               const AreaNode* current,
                               int             type,
                               int             ori);
    };
}  // namespace Jam::Editor
