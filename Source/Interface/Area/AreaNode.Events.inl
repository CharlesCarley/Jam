// ReSharper disable CppNonInlineFunctionDefinitionInHeaderFile
#pragma once
#include <QWidget>
#include "AreaNode.Accessors.inl"
#include "Interface/Area/AreaBranch.h"
#include "Interface/Area/AreaNode.h"
#include "Interface/Events/BranchEvent.h"
#include "Interface/Events/EventTypes.h"
#include "Interface/Events/SwitchEvent.h"
#include "Interface/Events/LayerSelectEvent.h"

namespace Jam::Editor
{
    bool AreaNode::propagate(QEvent* event) const
    {
        if (!event)
            return false;

        bool result = false;

        if (_content)
        {
            // Relay the event to the subtree
            // this call assumes that the event
            // travels from this node to all
            // subtrees starting with this
            result = _content->propagate(event) || result;
        }
        return result;
    }

    bool AreaNode::propagateFromRoot(QEvent* event) const
    {
        if (!event)
            return false;

        // find the top most node
        const AreaNode* root   = nullptr;
        const AreaNode* ascend = this;
        while (ascend)
        {
            root   = ascend;
            ascend = ascend->_parent;
        }

        // propagate to all subtrees.
        bool result = false;
        if (root)
            result = root->propagate(event);
        return result;
    }
    void AreaNode::handleSplitEvent(const BranchEvent* event)
    {
        // At the moment only leaf splits are available.
        // IE; The event is only sent via in AreaEdgeRect{l|r|t|b}
        if (isBranch() || !event)
            return;

        morphIntoBranch(event->from());

        // Recursively compute the edge masks
        // (An edge mask is defined by any l,t, r, or b edge
        // that does not share a border with the split widget
        // in a branch node).
        recomputeMask();
    }

    void AreaNode::handleMergeLeftEvent()
    {
        mergeLeft();
        recomputeMask();
    }

    void AreaNode::handleMergeRightEvent()
    {
        mergeRight();
        recomputeMask();
    }

    void AreaNode::handleSwitchEvent(const SwitchEvent* event)
    {
        if (isBranch() || !event)
            return;

        // swap content types
        switchContent(event->to());
        recomputeMask();
    }

    void AreaNode::handleBroadcastEvent(LayerSelectEvent* event) const
    {
        if (!event)
            return;

        propagateFromRoot(event);
    }

    bool AreaNode::event(QEvent* event)
    {
        if (!event)
            return false;

        switch ((int)event->type())
        {
        case SplitEvent:
            handleSplitEvent((BranchEvent*)event);
            return true;
        case MergeLeftEvent:
            handleMergeLeftEvent();
            return true;
        case MergeRightEvent:
            handleMergeRightEvent();
            return true;
        case ProjectChanged:
            return propagate(event);
        case SwitchContentEvent:
            handleSwitchEvent((SwitchEvent*)event);
            return true;
        case LayerSelect:
            handleBroadcastEvent((LayerSelectEvent*)event);
            return true;
        default:
            return QWidget::event(event);
        }
    }

}  // namespace Jam::Editor
