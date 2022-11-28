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
#include <functional>
#include <unordered_map>
#include "Utils/String.h"

namespace Jam::Xml
{
    class Node;
    class Attribute;

    using NodeSortFunc = std::function<bool(Node* a, Node* b)>;

    typedef std::unordered_map<String, String> AttributeMap;
    typedef std::vector<Node*>                 NodeArray;
    typedef std::unordered_map<String, Node*>  NodeMap;

    class Node
    {
    private:
        int64_t      _typeCode{-1};
        Node*        _parent{nullptr};
        Node*        _next{nullptr};
        String       _name;
        String       _text;
        AttributeMap _attributes;
        NodeArray    _children;
        bool         _childrenDetached{false};

    public:
        Node() = default;

        explicit Node(String name, int64_t = -1);

        ~Node();

        void addChild(Node* child);

        const NodeArray& children() const;

        void childrenOf(int type, NodeArray& dest) const;

        Node* at(const size_t& idx);

        const Node* at(const size_t& idx) const;

        size_t size() const;

        Node* parent() const;

        const String& name() const;

        const String& text() const;

        void text(const String& text);

        int64_t getTypeCode() const;

        void setTypeCode(int64_t code);

        const AttributeMap& attributes() const;

        const String& get(const String& attribute);

        bool contains(const String& attribute);

        void insert(const String& key, const String& v);
        void insert(const char* key, int v);
        void insert(const char* key, double v);

        void siblingsOf(NodeArray&, const String& tag) const;

        void clearChildren();

        void setDetachedState(bool detach);

        Node* firstChild() const;

        Node* getFirstChild(const String& requireType) const;

        Node* getFirstChild(const int64_t& requireType) const;

        Node* firstChildOf(const String& tag) const;

        Node* firstChildOf(const int64_t& tag) const;

        Node* firstParentOf(const String& tag);

        Node* firstParentOf(const int64_t& tag);

        Node* nextSiblingOf(const String& tag) const;

        Node* nextSiblingOf(const int64_t& tag) const;

        const String& attribute(const String& name, const String& def = "") const;

        int64_t integer(const String& name, int64_t def = -1) const;

        int64_t int64(const String& name, int64_t def = -1) const;

        int32_t int32(const String& name, int32_t def = -1) const;

        int16_t int16(const String& name, int16_t def = -1) const;

        float float32(const String& name, float def = 0.f) const;

        double float64(const String& name, double def = 0.f) const;

        bool isTypeOf(const char* tagName) const;

        bool isTypeOf(int64_t type) const;

        bool hasChildren() const;

        bool hasChild(const char* str) const;

        bool hasText() const;

        bool hasAttributes() const;

        void sort(const NodeSortFunc& fnc);
    };

    inline bool Node::isTypeOf(const int64_t type) const
    {
        return _typeCode == type;
    }

    inline const NodeArray& Node::children() const
    {
        return _children;
    }

    inline Node* Node::parent() const
    {
        return _parent;
    }

    inline const String& Node::name() const
    {
        return _name;
    }

    inline const String& Node::text() const
    {
        return _text;
    }

    inline void Node::text(const String& text)
    {
        _text = text;
    }

    inline const AttributeMap& Node::attributes() const
    {
        return _attributes;
    }

    inline bool Node::hasText() const
    {
        return !_text.empty();
    }

    inline bool Node::hasAttributes() const
    {
        return !_attributes.empty();
    }

    inline size_t Node::size() const
    {
        return _children.size();
    }

    inline int64_t Node::getTypeCode() const
    {
        return _typeCode;
    }

    inline void Node::setTypeCode(const int64_t code)
    {
        _typeCode = code;
    }

}  // namespace Jam::Xml
