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
            Q_ASSERT(_content == nullptr);
            setContent(new AreaLeaf(root, mask));
        }
    }

    void AreaNode::setContent(AreaContent* root)
    {
        // Provide the public setContent method.
        //
        // It involves linking
        // the content to the tree, then applying
        // the content to the view.
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
