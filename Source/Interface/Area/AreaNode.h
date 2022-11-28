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
#include "Interface/Events/BranchEvent.h"
#include "Interface/Areas/AreaType.h"

class QLayoutItem;
class QVBoxLayout;

namespace Jam::Editor
{
    class AreaCreator;
    class LayerSelectEvent;
    class SwitchEvent;
    class AreaContent;
    class AreaBranch;
    class Area;
    class AreaLeaf;

    class AreaNode final : public QWidget
    {
        Q_OBJECT
    private:
        AreaCreator* _creator;
        AreaNode*    _parent{nullptr};
        AreaNode*    _left{nullptr};
        AreaNode*    _right{nullptr};
        AreaContent* _content{nullptr};
        QVBoxLayout* _layout{nullptr};

        Area* wrappedArea();

        AreaBranch* pruneBranch();

        AreaLeaf* pruneLeaf();

        void unlinkPrunedObject(const QLayoutItem * item, AreaContent * content);

        void swapBranch(const AreaBranch* branch, const AreaNode* keepNode, const AreaNode* dropNode);

        void mergeLeft();

        void mergeRight();

        void linkContent(AreaContent* content);

        void updateViewContent(AreaContent* content);

        void switchContent(int to);

        void recomputeMask() const;

        void handleSplitEvent(const BranchEvent* event);

        void handleMergeLeftEvent();

        void handleMergeRightEvent();

        void handleSwitchEvent(const SwitchEvent * event);

        void handleBroadcastEvent(LayerSelectEvent* event) const;

        bool event(QEvent* event) override;

    public:
        explicit AreaNode(AreaCreator *creator, QWidget* parent = nullptr);
        ~AreaNode() override;

        void setArea(Area* root, int mask = 0x0F);

        void setContent(AreaContent* root);

        AreaContent* content() const;

        AreaBranch* branch() const;

        AreaLeaf* leaf() const;

        AreaNode* left() const;

        AreaNode* right() const;

        AreaNode* parentNode() const;

        void setLeft(AreaNode* left);

        void setRight(AreaNode* right);

        bool isBranch() const;

        bool isLeaf() const;

        bool hasLeft() const;

        bool hasRight() const;

        bool hasContent() const;

        void morphIntoBranch(const Qt::Orientation& direction);

        bool propagate(QEvent* event) const;

        bool propagateFromRoot(QEvent* event) const;
    };

    inline AreaContent* AreaNode::content() const
    {
        return _content;
    }

    inline AreaNode* AreaNode::left() const
    {
        return _left;
    }

    inline AreaNode* AreaNode::right() const
    {
        return _right;
    }

    inline AreaNode* AreaNode::parentNode() const
    {
        return _parent;
    }

    inline bool AreaNode::hasLeft() const
    {
        return _left != nullptr;
    }

    inline bool AreaNode::hasRight() const
    {
        return _right != nullptr;
    }

    inline bool AreaNode::hasContent() const
    {
        return _content != nullptr;
    }

}  // namespace Jam::Editor
