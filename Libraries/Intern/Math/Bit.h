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
#include <bitset>
#include "Math/Integer.h"

namespace Jam
{
    class Bit
    {
    public:
        U8 _value{0};

        Bit() = default;

        explicit Bit(const U8& v) :
            _value(v) {}

        void flip(U8 mask)
        {
            _value ^= ~mask;
        }

        Bit masked(const U8 mask) const
        {
            return Bit{
                (U8)(_value & mask),
            };
        }

        void zero()
        {
            _value ^= _value;
        }

        void print() const
        {
            const std::bitset<8> v(_value);
            Console::writeLine(v.to_string());
        }

        void inc(const U8 mask = 0b1111'1111,
                 const U8 mod  = 4)
        {
            const U8 a = _value & mask;
            _value &= ~mask;
            _value |= (a + 1) % mod;
        }
    };

}  // namespace MathJam
