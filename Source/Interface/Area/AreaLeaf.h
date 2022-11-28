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
#include "Interface/Area/AreaContent.h"
#include "Interface/Area/AreaEdgeRect.h"

class QHBoxLayout;
class QVBoxLayout;

namespace Jam::Editor
{
    class AreaEdge;
    class AreaNode;
    class Area;

    class AreaLeaf final : public AreaContent
    {
        Q_OBJECT

    signals:
        void split(Qt::Orientation current, Qt::Orientation dest);

    private:
        Area*         _leafInnerArea{nullptr};
        AreaEdgeRect* _edges{nullptr};
        int           _mask{AreaEdgeAll};

        void construct();

        void contextMenu(Qt::Orientation from, Qt::Orientation to, const QPoint& at);

        int computeMask();

    public:
        explicit AreaLeaf(Area* root, int mask = AreaEdgeAll);
        ~AreaLeaf() override;

        Area* contents() const;

        int32_t type() const;

        void updateMask() override;

        bool propagate(QEvent* event) override;
    };

}  // namespace Jam::Editor
