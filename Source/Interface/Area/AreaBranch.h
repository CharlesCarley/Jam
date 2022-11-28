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

class QSplitter;

namespace Jam::Editor
{
    class AreaBinaryLayout;
    class AreaNode;

    class AreaBranch final : public AreaContent
    {
        Q_OBJECT
    signals:
        void wantsToMerge(bool aIntoB);

    private:
        QVBoxLayout*      _layout{nullptr};
        AreaBinaryLayout* _splitter{nullptr};
        AreaNode*         _left{nullptr};
        AreaNode*         _right{nullptr};
        Qt::Orientation   _orientation;

        void construct();

        void contextMenu(const QPoint& at) const;

        void splitterMoved(const QPoint& at) const;

        bool propagate(QEvent* evt) override;

    public:
        AreaBranch(Qt::Orientation ori, AreaNode* left, AreaNode* right);

        ~AreaBranch() override;

        AreaNode* left() const;

        AreaContent* leftContent() const;

        AreaNode* right() const;

        AreaContent* rightContent() const;

        void dangleLayout() const;

        qreal ratio() const;

        Qt::Orientation orientation() const;

        void setRatio(qreal ratio) const;

        void updateMask() override;
    };

    inline AreaNode* AreaBranch::left() const
    {
        return _left;
    }

    inline AreaNode* AreaBranch::right() const
    {
        return _right;
    }

    inline Qt::Orientation AreaBranch::orientation() const
    {
        return _orientation;
    }

}  // namespace Jam::Editor
