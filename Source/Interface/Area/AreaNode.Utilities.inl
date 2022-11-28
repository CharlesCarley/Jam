// ReSharper disable CppNonInlineFunctionDefinitionInHeaderFile
#pragma once
#include <QBoxLayout>
#include <QWidget>

#include "AreaCreator.h"
#include "Interface/Area/Area.h"
#include "Interface/Area/AreaBranch.h"
#include "Interface/Area/AreaLeaf.h"
#include "Interface/Area/AreaNode.h"
#include "Interface/Areas/OutputArea.h"
#include "Interface/Extensions.h"

namespace Jam::Editor
{
    void AreaNode::recomputeMask() const
    {
        const AreaNode* root   = nullptr;
        const AreaNode* upNode = this;

        while (upNode)
        {
            root   = upNode;
            upNode = upNode->_parent;
        }

        if (root && root->hasContent())
            root->content()->updateMask();
    }

    void AreaNode::mergeLeft()
    {
        if (!_left || !_right)
            return;

        // The right node of this should be pruned,
        // and the left node should become this content.
        //
        // Therefore, this must be a branch to work.
        //
        // If the pruned node is a branch, the right and left nodes
        // in this need to be replaced with the pruned branches'
        // right and left nodes.
        //
        // If the pruned node is a leaf

        if (!isBranch())
        {
            Log::writeLine("Cannot merge a leaf into itself");
            return;
        }

        if (const AreaBranch* branch = pruneBranch(); branch != nullptr)
            swapBranch(branch, branch->left(), branch->right());
    }

    void AreaNode::mergeRight()
    {
        if (!_left || !_right)
            return;

        // The right node should be pruned, and the
        // left node should become this content
        if (!isBranch())
        {
            Log::writeLine("Cannot merge a leaf into itself");
            return;
        }

        if (const AreaBranch* branch = pruneBranch(); branch != nullptr)
            swapBranch(branch, branch->right(), branch->left());
    }

    void AreaNode::swapBranch(const AreaBranch* branch,
                              const AreaNode*   keepNode,
                              const AreaNode*   dropNode)
    {
        branch->dangleLayout();

        if (keepNode != nullptr)
            setContent(keepNode->content());

        delete branch;
        delete dropNode;
    }

    void AreaNode::morphIntoBranch(const Qt::Orientation& direction)
    {
        if (!isLeaf())
            return;
        if (const AreaLeaf* leaf = pruneLeaf())
        {
            Q_ASSERT(_layout->count() == 0);
            Q_ASSERT(_left == nullptr);
            Q_ASSERT(_right == nullptr);
            Q_ASSERT(_content == nullptr);

            Area* content = leaf->contents();
            if (content == nullptr)
                return;

            AreaNode* lhs = new AreaNode(_creator);
            lhs->setArea(content);

            AreaNode* rhs = new AreaNode(_creator);
            rhs->setArea(_creator->clone(content));

            setContent(new AreaBranch(direction, lhs, rhs));

            delete leaf;
        }
    }

    Area* AreaNode::wrappedArea()
    {
        if (AreaLeaf* areaNode = pruneLeaf())
        {
            areaNode->deleteLater();
            return areaNode->contents();
        }
        return nullptr;
    }

    AreaBranch* AreaNode::pruneBranch()
    {
        if (_layout && !_layout->isEmpty())
        {
            if (const QLayoutItem* item = _layout->takeAt(0);
                item && View::isBranch(item->widget()))
            {
                if (AreaBranch* branch = (AreaBranch*)item->widget())
                {
                    unlinkPrunedObject(item, branch);
                    return branch;
                }
                Log::writeLine("pruneBranch: failed to unlink branch");
            }
            else
                Log::writeLine(
                    "pruneBranch: the content in "
                    "the layout is not a branch");
        }
        return nullptr;
    }

    AreaLeaf* AreaNode::pruneLeaf()
    {
        if (_layout && !_layout->isEmpty())
        {
            if (const QLayoutItem* item = _layout->takeAt(0);
                item && View::isLeaf(item->widget()))
            {
                if (AreaLeaf* leaf = (AreaLeaf*)item->widget())
                {
                    unlinkPrunedObject(item, leaf);
                    return leaf;
                }
                Log::writeLine("pruneLeaf: failed to unlink leaf");
            }
            else
                Log::writeLine(
                    "pruneLeaf: the content in "
                    "the layout is not a leaf");
        }
        return nullptr;
    }

    void AreaNode::unlinkPrunedObject(const QLayoutItem* item, AreaContent* content)
    {
        Q_ASSERT(content != nullptr && content == _content);

        // Drop the item but keep the widget that is inside it.
        if (content)
            content->setParentNode(nullptr);
        delete item;
        _content = nullptr;
    }

    void AreaNode::updateViewContent(AreaContent* content)
    {
        if (!content)
        {
            Log::writeLine("updateViewContent: a null content was supplied.");
            return;
        }

        if (_content)
        {
            // disconnect all signals
            (void)_content->disconnect(this);
        }

        _content = content;

        // switch it's parent
        _content->setParentNode(this);

        if (!_layout)
        {
            _layout = new QVBoxLayout();
            View::layoutDefaults(_layout);
        }
        else
        {
            // it should be destroyed
            while (const QLayoutItem* item = _layout->takeAt(0))
            {
                delete item->widget();
                delete item;
            }
        }

        _layout->addWidget(_content, 1);
        setLayout(_layout);
    }

    void AreaNode::switchContent(const int to)
    {
        if (!_content || !_content->isLeaf())
            return;

        if (const AreaLeaf* leafNode = leaf();
            leafNode && leafNode->type() == to)
        {
            // Then there is no need to switch if it
            // is the same content type.
            return;
        }

        // Remove the current leaf content.
        delete pruneLeaf();

        // Instance it as the new type.
        setArea(_creator->fromType(to));
    }

    void AreaNode::linkContent(AreaContent* content)
    {
        if (!content)
        {
            Log::writeLine(
                "linkContent: an invalid content "
                "handle was supplied.");
            return;
        }

        if (content->isBranch())
        {
            const auto branch = (AreaBranch*)content;

            // link the supplied content's left and
            // right subtrees to this node.

            setLeft(branch->left());
            setRight(branch->right());
        }
        else
        {
            // null left and right indicates a leaf.
            _left  = nullptr;
            _right = nullptr;
        }

        // set the link back up the tree.
        content->setParentNode(this);
    }

}  // namespace Jam::Editor
