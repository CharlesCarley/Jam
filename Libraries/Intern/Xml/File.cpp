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
#include "Xml/File.h"
#include "Utils/Char.h"
#include "Xml/Node.h"
#include "Xml/Scanner.h"
#include "Xml/Token.h"
#include "Xml/TypeFilter.h"
#include "Xml/Writer.h"

namespace Jam::Xml
{
    File::File() :
        _root(new Node())
    {
        _scanner = new Scanner();
    }

    File::File(const TypeFilter* filter, const size_t filterSize) :
        _root(new Node())
    {
        _scanner = new Scanner();
        applyFilter(filter, filterSize);
    }

    File::~File()
    {
        if (_isAttached)
        {
            delete _root;
            _root = nullptr;
        }

        delete _scanner;
        _scanner = nullptr;
    }

    void File::applyFilter(const TypeFilter* filter, const size_t filterSize)
    {
        makeTypeFilter(_filter, filter, filterSize);
    }

    void File::errorMessageImpl(String& dest, const String& message)
    {
        OutputStringStream oss;
        oss << message << std::endl;

        while (_stack.size() > 1)
        {
            const Node* nd = _stack.top();
            oss << nd->name() << std::endl;

            _stack.pop();
            delete nd;
        }
        dest = oss.str();
    }

    Node* File::createTag(const String& name)
    {
        Node* node = new Node(name);
        _stack.push(node);
        return node;
    }

    Node& File::top()
    {
        if (_stack.empty())
            error("empty stack");
        return *_stack.top();
    }

    Node* File::tree() const
    {
        if (!_root)
            throw InvalidPointer();
        return _root;
    }

    Node* File::root(const char* name) const
    {
        return tree()->firstChildOf(name);
    }

    Node* File::detachRoot()
    {
        _isAttached      = false;
        Node* detachment = _root;
        _root            = nullptr;
        return detachment;
    }

    Node* File::root(const int64_t code) const
    {
        return tree()->firstChildOf(code);
    }

    void File::reduceRule()
    {
        if (_stack.size() > 1)
        {
            Node* b = _stack.top();
            _stack.pop();

            if (_filter.empty())
            {
                Node* a = _stack.top();
                a->addChild(b);
            }
            else
            {
                if (const auto it = _filter.find(b->name());
                    it != _filter.end())
                {
                    Node* a = _stack.top();
                    b->setTypeCode(it->second);
                    a->addChild(b);
                }
                else
                    delete b;
            }
        }
    }

    void File::dropRule()
    {
        if (_stack.size() > 1)
        {
            const Node* b = _stack.top();
            _stack.pop();
            delete b;
        }
    }

    void File::ruleAttributeList()
    {
        int8_t t0 = token(0).type();

        if (t0 != TOK_EN_TAG && t0 != TOK_SLASH)
        {
            do
            {
                ruleAttribute();
                t0 = token(0).type();

                if (t0 == TOK_EOF)
                    error("unexpected end of file");

            } while (t0 != TOK_EN_TAG && t0 != TOK_SLASH);
        }
    }

    void File::ruleAttribute()
    {
        const int8_t t0 = token(0).type();
        const int8_t t1 = token(1).type();
        const int8_t t2 = token(2).type();

        if (t0 != TOK_IDENTIFIER)
            error("expected an identifier");
        if (t1 != TOK_EQUALS)
            error("expected an equals sign");
        if (t2 != TOK_STRING)
            error("expected an equals sign");

        Node& node = top();

        String identifier;
        _scanner->string(identifier, token(0).index());

        if (node.contains(identifier))
            error(node.name(), " duplicate attribute ", identifier);

        String value;
        _scanner->string(value, token(2).index());

        node.insert(identifier, value);
        advanceCursor(3);
    }

    void File::ruleXmlRoot()
    {
        int8_t       t0 = token(0).type();
        const int8_t t1 = token(1).type();
        const int8_t t2 = token(2).type();

        if (t0 != TOK_ST_TAG)
            error("expected the '<' character");
        if (t1 != TOK_QUESTION)
            error("expected the '/' character");
        if (t2 != TOK_KW_XML)
            error("expected the xml keyword");

        advanceCursor(3);
        t0 = token(0).type();

        while (t0 != TOK_QUESTION)
        {
            ruleAttribute();
            t0 = token(0).type();
            if (t0 == TOK_EOF)
                error("unexpected end of file");
        }

        advanceCursor();
        t0 = token(0).type();
        if (t0 != TOK_EN_TAG)
            error("unexpected token ", Char::toHexString((uint8_t)t0));
        advanceCursor();
    }

    void File::ruleStartTag()
    {
        const Token& t0 = token(0);
        const Token& t1 = token(1);

        if (t0.type() != TOK_ST_TAG)
            error("expected the < character");
        if (t1.type() != TOK_IDENTIFIER)
            error("expected a tag identifier");

        String value;
        _scanner->string(value, t1.index());
        if (value.empty())
            error("empty tag name");

        advanceCursor(2);

        createTag(value);

        ruleAttributeList();

        // Test exit state from the attribute list call
        // > means leave node on the stack
        // / means remove the node from the stack

        const int8_t et0 = token(0).type();

        if (et0 == TOK_SLASH)
        {
            const int8_t et1 = token(1).type();
            if (et1 != TOK_EN_TAG)
                error("expected the '>' character ");

            reduceRule();
            advanceCursor(2);
        }
        else if (et0 != TOK_EN_TAG)
            error("expected the '>' character ");
        else
            advanceCursor();
    }

    void File::ruleContent()
    {
        const Token& t0 = token(0);
        if (t0.type() != TOK_TEXT)
            error("expected content text");

        String content;

        auto* scn = (Scanner*)_scanner;
        scn->getCode(content, t0.index());

        if (content.empty())
            error("unexpected empty content token");

        top().text(content);

        Node* node = createTag("_text_node");
        node->text(content);
        reduceRule();

        advanceCursor();
    }

    void File::ruleEndTag()
    {
        // '<' '/'
        const int8_t t0 = token(0).type();
        const int8_t t1 = token(1).type();
        const int8_t t2 = token(2).type();
        const int8_t t3 = token(3).type();

        if (t0 != TOK_ST_TAG)
            error("expected the '<' character");
        if (t1 != TOK_SLASH)
            error("expected the '/' character");
        if (t2 != TOK_IDENTIFIER)
            error("expected a tag identifier");
        if (t3 != TOK_EN_TAG)
            error("expected the '>' character");

        String identifier;
        _scanner->string(identifier, token(2).index());

        if (identifier != top().name())
        {
            error("closing tag mis-match between '",
                  top().name(),
                  '\'',
                  " and '",
                  identifier,
                  '\'');
        }

        if (identifier.empty())
            error("empty closing tag");

        advanceCursor(4);
        reduceRule();
    }

    void File::ruleObject()
    {
        const int8_t t0 = token(0).type();
        const int8_t t1 = token(1).type();
        const int8_t t2 = token(2).type();

        if (t0 == TOK_ST_TAG && t1 == TOK_IDENTIFIER)
            ruleStartTag();
        else if (t0 == TOK_ST_TAG && t1 == TOK_SLASH && t2 == TOK_IDENTIFIER)
            ruleEndTag();
        else
            ruleContent();
    }

    void File::ruleObjectList()
    {
        const int8_t t0 = token(0).type();
        const int8_t t1 = token(1).type();

        if (t1 == TOK_QUESTION)
        {
            createTag("xml");

            ruleXmlRoot();

            // If this actually controlled the parser
            // set it here, but it's not so just drop it.
            dropRule();
        }
        else if (t0 == TOK_ST_TAG || t0 == TOK_TEXT)
            ruleObject();
        else
            error("unknown token parsed 0x", Char::toHexString((uint8_t)t0));
    }

    void File::parseImpl(IStream& input)
    {
        // make sure the token cursor is at zero
        // initially and attach the input stream
        // to the scanner
        _cursor = 0;
        _scanner->attach(&input, PathUtil(_file));

        _stack.push(_root);

        while (_cursor <= (int32_t)_tokens.size())
        {
            if (const int8_t tok = token(0).type();
                tok == TOK_EOF)
                break;

            const int32_t op = _cursor;
            ruleObjectList();

            // if the cursor did not
            // advance force it to.
            if (op == _cursor)
                advanceCursor();
        }
    }

    void File::writeImpl(OStream& output, int format)
    {
        if (_root)
        {
            if (Node* wn = _root->children().front())
            {
                Writer writer(wn);

                if (format & Minify)
                    writer.setMinify(true);
                else
                    writer.setMinify(false);

                if (format & Indent2)
                    writer.setIndent(2);
                else if (format & Indent4)
                    writer.setIndent(4);
                else
                    writer.setIndent(0);

                writer.setShowXmlHeader(false);
                writer.write(output);
            }
        }
    }

    Node* File::constructClone(
        const Node*       root,
        const TypeFilter* filter,
        const size_t      filterSize)
    {
        if (!root)
            return nullptr;

        String ret;
        Writer::toString(ret, root);

        if (!ret.empty())
        {
            StringStream ss;
            ss << ret;

            File fp(filter, filterSize);
            fp.read(ss);

            return fp.detachRoot();
        }
        return nullptr;
    }

}  // namespace Jam::Xml
