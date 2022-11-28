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
#include "Math/Vec2F.h"

namespace Jam
{
    class Box;

    class EdgeList
    {
    public:
        Vec2F e[4]{};

        explicit EdgeList(const Box& b);

        const Vec2F& e1() const
        {
            return e[0];
        }
        const Vec2F& e2() const
        {
            return e[1];
        }
        const Vec2F& e3() const
        {
            return e[2];
        }
        const Vec2F& e4() const
        {
            return e[3];
        }
    };
}  // namespace Jam
