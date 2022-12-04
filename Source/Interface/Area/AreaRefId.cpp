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
#include "Interface/Area/AreaRefId.h"
#include "Utils/Definitions.h"

namespace Jam::Editor
{
    size_t AreaRefId::_ref = 0;
    size_t AreaRefId::_min = 0;
    size_t AreaRefId::_max = 0;

    size_t AreaRefId::newRef()
    {
        return ++_ref;
    }

    size_t AreaRefId::updateRef(size_t size)
    {
        size_t rv;
        if (size == JtNpos)  // new type from ui
            rv = newRef();
        else
            rv = size;  // type from load

        _min = Min(_min, rv);
        _max = Max(_max, rv);
        return rv;
    }

}  // namespace Jam::Editor
