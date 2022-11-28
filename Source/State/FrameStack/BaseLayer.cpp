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
#include "State/FrameStack/BaseLayer.h"
#include "State/FrameStack/FrameStack.h"

namespace Jam
{
    BaseLayer::BaseLayer(const LayerType type) :
        type(type)
    {
    }

    bool BaseLayer::resizeEvent(const Vec2I& size)
    {
        return false;
    }

    void BaseLayer::render(RenderContext& canvas)
    {
    }

    bool BaseLayer::update()
    {
        return false;
    }

    bool BaseLayer::injectText(const String& text)
    {
        // nadda
        return false;
    }

    bool BaseLayer::injectVec2FImpl(const FrameStackCode& code,
                                    const Vec2F&          size)
    {
        return false;
    }

    bool BaseLayer::injectVec2(const FrameStackCode& code,
                               const Vec2F&          size)
    {
        switch (code)
        {
        case SIZE:
            if (_size != toVec2I(size))
            {
                _size = toVec2I(size);
                return resizeEvent(_size);
            }
            return false;
        case X_AXIS:
        case Y_AXIS:
        case X_STEP:
        case Y_STEP:
        case ORIGIN:
        default:
            return injectVec2FImpl(code, size);
        case FUNC_TEXT:
            return false;
        }
    }

}  // namespace Jam
