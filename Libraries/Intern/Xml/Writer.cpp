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
#include "Xml/Writer.h"
#include <iomanip>
#include "Utils/Exception.h"
#include "Utils/FileSystem.h"
#include "Xml/Node.h"

namespace Jam::Xml
{
    constexpr size_t Indent = 2;

    Writer::Writer(const Node* root) :
        _root(root),
        _indentBy{Indent}
    {
    }

    void Writer::openTag(const Node* tag)
    {
        if (!tag)
            return;

        if (_notMinify)
        {
            if (_indent > 0)
                _out << std::setw((size_t)_indent) << ' ';
        }

        _out << '<' << tag->name();
        writeAttributes(tag);
        _out << '>';

        if (_notMinify && !tag->hasText())
            _out << std::endl;
    }

    void Writer::closeTag(const Node* tag)
    {
        if (!tag)
            return;

        if (_notMinify)
        {
            if (!tag->hasText() && (int)_indent > 0)
                _out << std::setw(_indent) << ' ';
        }

        _out << '<' << '/' << tag->name() << '>';

        if (_notMinify)
            _out << std::endl;
    }

    void Writer::inlineTag(const Node* tag)
    {
        if (!tag)
            return;

        if (_notMinify)
            _out << std::setw(_indent) << ' ';

        _out << '<'
             << tag->name();

        writeAttributes(tag);

        _out << '/' << '>';

        if (_notMinify)
            _out << std::endl;
    }

    void Writer::writeAttributes(const Node* tag)
    {
        if (!tag)
            return;

        if (tag->hasAttributes())
        {
            const AttributeMap& attr = tag->attributes();
            for (const auto& [k, v] : attr)
            {
                _out << ' ';
                _out << k << '=' << '"' << v << '"';
            }
        }
    }

    void Writer::writeFullTag(const Node* tag)
    {
        if (!tag)
            return;

        openTag(tag);
        _out << tag->text();

        for (const Node* element : tag->children())
            writeTag(element);

        closeTag(tag);
    }

    void Writer::writeTag(const Node* tag)
    {
        if (!tag)
            return;

        _indent += _indentBy;

        if (tag->hasChildren() || tag->hasText())
            writeFullTag(tag);
        else
            inlineTag(tag);

        _indent -= _indentBy;
    }

    void Writer::write(OStream& output)
    {
        if (_writeXml)
            _out << "<?xml version=\"1.0\"?>" << std::endl;

        _indent = (_indentOffset - _indentBy);
        writeTag(_root);

        const String dest = _out.str();
        output.write(dest.c_str(), (std::streamsize)dest.size());
    }

    void Writer::write(const String& path)
    {
        std::ofstream os(path);
        if (!os.is_open())
            throw Exception("Failed to open the input file '", path, "'");

        write(os);
    }

    void Writer::toString(String&     dest,
                          const Node* root,
                          const bool  minify,
                          const I32   indent,
                          const I32   offset)
    {
        OutputStringStream oss;
        toStream(oss, root, minify, indent, offset);
        dest = oss.str();
    }

    void Writer::toStream(OStream&    dest,
                          const Node* root,
                          const bool  minify,
                          const I32   indent,
                          const I32   offset)
    {
        Writer writer(root);
        writer.setMinify(minify);
        writer.setShowXmlHeader(false);
        writer.setIndent(indent);
        writer.setIndentOffset(offset);
        writer.write(dest);
    }
}  // namespace Jam::Xml
