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
#include "Math/Integer.h"
#include "Utils/Definitions.h"
#include "Utils/String.h"

namespace Jam::Xml
{
    class Node;

    enum WriteFormat
    {
        Minify  = 0x01,
        Indent2 = 0x02,
        Indent4 = 0x04,
    };

    /**
     * \brief Is a utility class that is used to write the xml
     * text structure to the supplied stream from the
     * supplied root node.
     */
    class Writer
    {
    private:
        const Node*        _root;
        OutputStringStream _out;
        int32_t            _indentBy{2};
        int32_t            _indentOffset{0};
        int32_t            _indent{0};
        bool               _writeXml{true};
        bool               _notMinify{false};

        void openTag(const Node* tag);

        void closeTag(const Node* tag);

        void inlineTag(const Node* tag);

        void writeAttributes(const Node* tag);

        void writeFullTag(const Node* tag);

        void writeTag(const Node* tag);

    public:
        explicit Writer(const Node* root);

        ~Writer() = default;

        void setIndent(int indent);
        void setIndentOffset(int indent);
        void setShowXmlHeader(bool value);
        void setMinify(bool value);

        void write(OStream& output);
        void write(const String& path);

        static void toString(String&     dest,
                             const Node* root,
                             bool        minify = true,
                             I32         indent = 4,
                             I32         offset = 0);

        static void toStream(OStream&    dest,
                             const Node* root,
                             bool        minify = true,
                             I32         indent = 4,
                             I32         offset = 0);
    };

    inline void Writer::setIndent(const int indent)
    {
        _indentBy = Clamp(indent, 1, 16);
    }

    inline void Writer::setIndentOffset(const int indent)
    {
        _indentOffset = Clamp(indent, 0, 80);
    }

    inline void Writer::setShowXmlHeader(const bool value)
    {
        _writeXml = value;
    }

    inline void Writer::setMinify(bool value)
    {
        _notMinify = !value;
    }

}  // namespace Jam::Xml
