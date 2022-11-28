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

#include "Integer.h"
#include "Math/Real.h"

namespace Jam
{
    class Mat4
    {
    public:
        R32 m[4][4]{
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1},
        };
        Mat4() = default;

        explicit Mat4(const R32 v[4][4])
        {
            for (I8 i = 0; i < 4; ++i)
            {
                for (I8 j = 0; j < 4; ++j)
                    m[i][j] = v[i][j];
            }
        }

        const R32* ptr() const
        {
            return &m[0][0];
        }

        void transpose();
    };

}  // namespace Jam
