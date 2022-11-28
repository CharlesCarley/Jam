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
#include "XmlConverter.h"
#include "Interface/PersistentSettings.h"
#include "Math/Box.h"
#include "Math/Color.h"
#include "Math/RectF.h"
#include "Math/Vec2F.h"
#include "StringConverter.h"
#include "Utils/Char.h"
#include "Utils/String.h"
#include "Xml/Node.h"
#include "Xml/Writer.h"

namespace Jam
{
    Color XmlConverter::toColor(const XmlNode* tag, const Color& def)
    {
        Color copy = def;
        if (tag != nullptr)
            StringConverter::toColor(tag->text(), copy);
        return copy;
    }

    R32Array XmlConverter::toRealArray(const XmlNode* tag, const R32Array& def)
    {
        R32Array copy;
        if (tag != nullptr)
            StringConverter::toR32Array(tag->text(), copy);
        return copy;
    }

    Vec2F XmlConverter::toVec2F(const XmlNode* tag, const Vec2F& def)
    {
        Vec2F copy = def;
        if (tag != nullptr)
            StringConverter::toVec2F(tag->text(), copy);
        return copy;
    }

    Vec2F XmlConverter::toVec2F(const String&  attr,
                                const XmlNode* tag,
                                const Vec2F&   def,
                                const Vec2F&   minMax)
    {
        Vec2F copy = def;
        if (tag != nullptr)
        {
            if (const String value = tag->attribute(attr);
                !value.empty())
            {
                StringConverter::toVec2F(value, copy);
                copy.clamp(minMax);
            }
            // use default quietly...
        }
        return copy;
    }

    R32 XmlConverter::toReal(const XmlNode* tag, const R32& def)
    {
        if (tag != nullptr)
            return Char::toFloat(tag->text(), def);
        return def;
    }

    int32_t XmlConverter::toInt(const XmlNode* tag, const int32_t& def)
    {
        if (tag != nullptr)
            return Char::toInt32(tag->text(), def);
        return def;
    }

    RectF XmlConverter::toRect(const XmlNode* tag, const RectF& def)
    {
        RectF copy = def;
        if (tag != nullptr)
            StringConverter::toRectF(tag->text(), copy);
        return copy;
    }

    Box XmlConverter::toBox(const XmlNode* tag, const Box& def)
    {
        Box copy = def;
        if (tag != nullptr)
            StringConverter::toBox(tag->text(), copy);
        return copy;
    }

    int32_t XmlConverter::toOrEnum(const XmlNode* tag,
                                   const EnumMap* values,
                                   const size_t&  size,
                                   const int32_t& def)
    {
        if (tag == nullptr)
            return def;

        if (const String& text = tag->text();
            !text.empty())
        {
            StringArray stringArray;
            StringUtils::splitRejectEmpty(stringArray, text, ',');

            int32_t copy = 0;

            for (const String& supplied : stringArray)
            {
                for (size_t i = 0; i < size; ++i)
                {
                    if (const auto& [lookup, value] = values[i];
                        Char::equals(lookup, supplied.c_str(), Char::length(lookup)))
                        copy |= value;
                }
            }
            return copy;
        }
        return def;
    }

    void XmlConverter::toIntArray(const XmlNode* tag, I32Array& dest)
    {
        if (tag != nullptr)
            StringConverter::toI32Array(tag->text(), dest);
    }

    void XmlConverter::toIntArray(const String&  attr,
                                  const XmlNode* tag,
                                  I32Array&      dest)
    {
        if (tag != nullptr)
        {
            if (const String val = tag->attribute(attr);
                !val.empty())
                StringConverter::toI32Array(val, dest);
        }
    }

    Axis XmlConverter::toAxis(const String&  attr,
                              const XmlNode* tag,
                              const Slice&   defX,
                              const Slice&   defY)
    {
        Axis dest;
        I32Array v;
        toIntArray(attr, tag, v);

        if (v.size() == 4)
        {
            dest.x = {v[0], v[1]};
            dest.y = {v[2], v[3]};
        }
        else
        {
            dest.x = defX;
            dest.y = defY;
        }
        return dest;
    }

    uint32_t XmlConverter::toIntFromBinary(const XmlNode* tag)
    {
        uint32_t result = 0;

        if (tag != nullptr)
        {
            const String& txt = tag->text();
            int           v   = 0;

            for (int i = (int)txt.size() - 1; i >= 0; --i)
            {
                if (const char ch = txt[i]; ch == '0' || ch == '1')
                {
                    if (ch == '1')
                        result |= (1 << v);
                    v++;
                }
                // else TODO: warn invalid input.
            }
        }
        return result;
    }

    void XmlConverter::toStream(OStream&       stream,
                                const XmlNode* fromRoot,
                                const I8       indent)
    {
        const Editor::PersistentSettings settings;

        Xml::Writer log(fromRoot);
        log.setShowXmlHeader(false);
        log.setMinify(settings.minify());
        log.setIndent(settings.spaces());
        log.setIndentOffset(indent);
        log.write(stream);
    }

    void XmlConverter::toString(String&        str,
                                const XmlNode* fromRoot,
                                const I8       indent)
    {
        OutputStringStream oss;
        toStream(oss, fromRoot, indent);
        str = oss.str();
    }
}  // namespace Jam
