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

#include <qstring.h>
#include "Interface/Areas/AreaType.h"
#include "Interface/Events/EventTypes.h"
#include "Math/Integer.h"
#include "State/FrameStack/BaseLayer.h"

namespace Jam::Editor
{
    class LayerSelectEvent final : public QEvent
    {
    private:
        LayerType _type{NoType};
        U32       _index{JtNpos32};

    public:
        explicit LayerSelectEvent(LayerType type, U32 idx) :
            QEvent((Type)(int)LayerSelect),
            _type(type),
            _index(idx)
        {
        }

        const LayerType& layerType() const
        {
            return _type;
        }

        const U32& stackIndex() const
        {
            return _index;
        }
    };

}  // namespace Jam::Editor
