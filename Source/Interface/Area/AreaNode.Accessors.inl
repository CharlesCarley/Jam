// ReSharper disable CppMissingIncludeGuard
// ReSharper disable CppNonInlineFunctionDefinitionInHeaderFile
#pragma once
#include "Interface/Area/Area.h"
#include "Interface/Area/AreaBranch.h"
#include "Interface/Area/AreaLeaf.h"
#include "Interface/Area/AreaNode.h"

namespace Jam::Editor
{
    void AreaNode::setArea(Area* root, const int mask)
    {
        if (root)
        {
            //connect(root, &Area::propagateEvent, this, [=](QEvent* evt)
            //{
            //    propagateFromRoot(evt);
            //    delete evt;
            //});

            // For now make sure this is
            // invalid before applying. why?

            Q_ASSERT(_content == nullptr);
            setContent(new AreaLeaf(root, mask));
        }
    }

    void AreaNode::setContent(AreaContent* root)
    {
        // Provide the public setContent method. It involves linking
        // the content to the tree, then applying the content to the view. 
        linkContent(root);

        updateViewContent(root);
    }

    AreaBranch* AreaNode::branch() const
    {
        if (_content && _content->isBranch())
            return (AreaBranch*)_content;
        return nullptr;
    }

    AreaLeaf* AreaNode::leaf() const
    {
        if (_content && _content->isLeaf())
            return (AreaLeaf*)_content;
        return nullptr;
    }
    

    void AreaNode::setLeft(AreaNode* left)
    {
        _left = left;
        if (_left)
            _left->_parent = this;
    }

    void AreaNode::setRight(AreaNode* right)
    {
        _right = right;
        if (_right)
            _right->_parent = this;
    }

    bool AreaNode::isBranch() const
    {
        if (_content)
            return _content->isBranch();
        return false;
    }

    bool AreaNode::isLeaf() const
    {
        if (_content)
            return _content->isLeaf();
        return false;
    }
    
}  // namespace Jam::Editor
