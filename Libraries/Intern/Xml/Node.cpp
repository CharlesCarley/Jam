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
#include "Xml/Node.h"
#include <algorithm>
#include <utility>
#include "Utils/Char.h"
#include "Utils/Exception.h"
#include "Utils/ParserBase/ParserBase.h"

namespace Jam::Xml
{
    using AttributeIt = AttributeMap::const_iterator;

    Node::Node(String name, const int64_t typeCode) :
        _typeCode(typeCode),
        _name(std::move(name))
    {
    }

    Node::~Node()
    {
        clearChildren();
    }

    void Node::addChild(Node* child)
    {
        if (!child)
            throw MessageException("invalid node supplied to node.addChild");

        child->_parent = this;
        child->_next   = nullptr;

        if (!_children.empty())
            _children.back()->_next = child;

        _children.push_back(child);
    }

    void Node::childrenOf(const int type, NodeArray& dest) const
    {
        if (!_children.empty())
        {
            for (Node* chi : _children)
            {
                if (chi->isTypeOf(type))
                    dest.push_back(chi);
            }
        }
    }

    Node* Node::at(const size_t& idx)
    {
        if (idx >= _children.size())
            return nullptr;

        return _children.at(idx);
    }

    const Node* Node::at(const size_t& idx) const
    {
        if (idx >= _children.size())
            return nullptr;

        return _children.at(idx);
    }

    bool Node::contains(const String& attribute)
    {
        return _attributes.find(attribute) != _attributes.end();
    }

    void Node::insert(const String& key, const String& v)
    {
        if (const AttributeIt it = _attributes.find(key);
            it == _attributes.end())
            _attributes.insert(std::make_pair(key, v));

        // TODO: Warn?
    }

    void Node::insert(const char* key, const int v)
    {
        if (key && *key)
            insert(key, Char::toString(v));
    }

    void Node::insert(const char* key, const double v)
    {
        if (key && *key)
            insert(key, Char::toString(v));
    }

    const String& Node::get(const String& attribute)
    {
        if (const AttributeIt it = _attributes.find(attribute);
            it != _attributes.end())
            return it->second;
        throw NotFound();
    }

    const String& Node::attribute(const String& name, const String& def) const
    {
        if (const AttributeIt it = _attributes.find(name);
            it != _attributes.end())
            return it->second;
        return def;
    }

    int64_t Node::integer(const String& name, int64_t def) const
    {
        if (const String val = attribute(name); !val.empty())
            return Char::toInt64(val, def);
        return def;
    }

    int64_t Node::int64(const String& name, int64_t def) const
    {
        return integer(name, def);
    }

    int32_t Node::int32(const String& name, int32_t def) const
    {
        return (int32_t)integer(name, (int64_t)def);
    }

    int16_t Node::int16(const String& name, int16_t def) const
    {
        return (int16_t)integer(name, (int64_t)def);
    }

    float Node::float32(const String& name, float def) const
    {
        if (const String val = attribute(name); !val.empty())
            return Char::toFloat(val, def);
        return def;
    }

    double Node::float64(const String& name, double def) const
    {
        if (const String val = attribute(name); !val.empty())
            return Char::toDouble(val, def);
        return def;
    }

    bool Node::isTypeOf(const char* tagName) const
    {
        if (!tagName)
            throw MessageException("invalid string supplied");

        return Char::equals(_name.c_str(),
                            tagName,
                            std::max(_name.size(), Char::length(tagName)));
    }

    bool Node::hasChildren() const
    {
        return !_children.empty();
    }

    bool Node::hasChild(const char* str) const
    {
        if (!str)
            throw InvalidPointer();

        for (const Node* child : _children)
        {
            if (child->isTypeOf(str))
                return true;
        }
        return false;
    }

    void Node::sort(const NodeSortFunc& fnc)
    {
        // Use stable_sort to preserve any
        // order that is already there.
        std::stable_sort(_children.begin(), _children.end(), fnc);
    }

    void Node::siblingsOf(NodeArray& dest, const String& tag) const
    {
        if (tag.empty())
            throw MessageException("the supplied tag can not be empty");

        for (Node* child : _children)
        {
            if (child->isTypeOf(tag.c_str()))
                dest.push_back(child);
        }
    }

    void Node::clearChildren()
    {
        if (_childrenDetached)
        {
            _children.clear();
        }
        else
        {
            for (const Node* child : _children)
                delete child;
            _children.clear();
        }
    }

    void Node::setDetachedState(bool detach)
    {
        _childrenDetached = detach;
    }

    Node* Node::firstChild() const
    {
        if (!_children.empty())
            return _children.front();

        return nullptr;
    }

    Node* Node::getFirstChild(const String& requireType) const
    {
        if (_children.empty())
        {
            throw InputException(
                "The supplied node has no children, "
                "therefore the first child '",
                requireType,
                "' of it is undefined");
        }

        Node* chi = _children.at(0);

        if (!requireType.empty() && chi->name() != requireType)
        {
            throw InputException(
                "the top level node is not "
                "a ",
                requireType,
                "node");
        }
        return chi;
    }

    Node* Node::getFirstChild(const int64_t& requireType) const
    {
        if (_children.empty())
        {
            throw InputException(
                "The supplied node has no children, "
                "therefore the first child '",
                requireType,
                "' of it is undefined");
        }

        Node* chi = _children.at(0);
        if (chi->getTypeCode() != requireType)
        {
            throw InputException(
                "the top level node is not "
                "a ",
                requireType,
                "node");
        }
        return chi;
    }

    Node* Node::firstChildOf(const String& tag) const
    {
        if (tag.empty())
            throw MessageException("the supplied tag can not be empty");

        for (Node* child : _children)
        {
            if (child->isTypeOf(tag.c_str()))
                return child;
        }
        return nullptr;
    }

    Node* Node::firstParentOf(const String& tag)
    {
        Node* cur = this;
        while (cur)
        {
            if (cur->isTypeOf(tag.c_str()))
                break;
            cur = cur->_parent;
        }
        return cur;
    }

    Node* Node::firstParentOf(const int64_t& tag)
    {
        Node* cur = this;
        while (cur)
        {
            if (cur->isTypeOf(tag))
                break;
            cur = cur->_parent;
        }
        return cur;
    }

    Node* Node::firstChildOf(const int64_t& tag) const
    {
        for (Node* child : _children)
        {
            if (child->isTypeOf(tag))
                return child;
        }
        return nullptr;
    }

    Node* Node::nextSiblingOf(const String& tag) const
    {
        if (tag.empty())
            throw MessageException("the supplied tag can not be empty");

        Node* nd = _next;
        while (nd != nullptr)
        {
            if (nd->isTypeOf(tag.c_str()))
                return nd;
            nd = nd->_next;
        }
        return nullptr;
    }

    Node* Node::nextSiblingOf(const int64_t& tag) const
    {
        Node* nd = _next;
        while (nd != nullptr)
        {
            if (nd->isTypeOf(tag))
                return nd;
            nd = nd->_next;
        }
        return nullptr;
    }

}  // namespace Jam::Xml
