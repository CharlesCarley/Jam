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
#include "Math/Axis.h"
#include "Math/Box.h"
#include "Math/Color.h"
#include "Math/Real.h"
#include "Math/RectF.h"
#include "Math/Vec2F.h"
#include "Xml/Declarations.h"

namespace Jam
{
    struct EnumMap
    {
        const char* lookup;
        int32_t     value;
    };

    class XmlConverter
    {
    public:
        static Vec2F toVec2F(const XmlNode* tag,
                             const Vec2F&   def = {R32(0), 0});

        static Vec2F toVec2F(const String&  attr,
                             const XmlNode* tag,
                             const Vec2F&   def    = {R32(0), 0},
                             const Vec2F&   minMax = {UnitMin, UnitMax});

        static R32 toReal(const XmlNode* tag, const R32& def = 0);

        static int32_t toInt(const XmlNode* tag, const int32_t& def = 0);

        static Color toColor(const XmlNode* tag, const Color& def = {1, 0, 1, 1});

        static Box toBox(const XmlNode* tag, const Box& def = {0, 0, 0, 0});

        static RectF toRect(const XmlNode* tag, const RectF& def = {0, 0, 0, 0});

        static R32Array toRealArray(const XmlNode* tag, const R32Array& def = {});

        static int32_t toOrEnum(const XmlNode* tag, const EnumMap* values, const size_t& size, const int32_t& def = 0);

        static void toIntArray(const XmlNode* tag, I32Array& dest);
        static void toIntArray(const String&  attr,
                               const XmlNode* tag,
                               I32Array&      dest);

        static Axis toAxis(const String&  attr,
                           const XmlNode* tag,
                           const Slice&   defX = {1, 1},
                           const Slice&   defY = {1, 1});

        static uint32_t toIntFromBinary(const XmlNode* tag);

        static void toStream(OStream& stream, const XmlNode* fromRoot, I8 indent = 0);
        static void toString(String& str, const XmlNode* fromRoot, I8 indent = 0);
    };
}  // namespace Jam
