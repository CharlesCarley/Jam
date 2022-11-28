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
#include "Math/Box.h"
#include "Math/Color.h"
#include "Math/RectF.h"
#include "Math/Vec2F.h"
#include "Utils/String.h"

namespace Jam
{
    constexpr U16 MaxSplit = 0x100; 


    class StringConverter
    {
    public:
        static void toVec2F(const String& str, Vec2F& dest);
        static void toColor(const String& txt, Color& dest);
        static void toRectF(const String& str, RectF& dest);
        static void toBox(const String& str, Box& dest);

        static void toR32Array(const String& str, R32Array& dest, I8 sep=',');
        static void toI32Array(const String& str, I32Array& dest, I8 sep=',');
    };
}  // namespace Jam
