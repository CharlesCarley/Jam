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
#include "Utils/Exception.h"

namespace Jam::Eq
{
    class CallState
    {
    private:
        const I32 _max{-1};
        I16       _depth{0};
        I16       _comma{0};

        CallState()                 = default;
        CallState(const CallState&) = default;

    public:
        explicit CallState(const I16& max) :
            _max(max) {}

        void depthGuard()
        {
            if (++_depth > _max || _depth < 0)
                throw Exception(
                    "maximum recursion depth exceeded");
        }

        void resetGuard() { _depth = 0; }

        void setCommaCount(const I16 c) { _comma = c; }

        const I16& commaCount() const { return _comma; }
    };

}  // namespace Jam::Eq
