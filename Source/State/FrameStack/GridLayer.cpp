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
#include "State/FrameStack/GridLayer.h"
#include "State/FrameStack/RenderContext.h"

namespace Jam::Layers
{

    GridLayer::GridLayer() :
        BaseLayer(GridType)
    {
    }

    bool GridLayer::resizeEvent(const Vec2I&)
    {
        _origin.x = _size.rx() * Half;
        _origin.y = _size.ry() * Half;
        return false;
    }

    bool GridLayer::injectVec2FImpl(const FrameStackCode& code, const Vec2F& size)
    {
        switch (code)
        {
        case X_AXIS:
            _axis.set(0, size);
            return true;
        case Y_AXIS:
            _axis.set(1, size);
            return true;
        case X_STEP:
            _axis.x.stepI(size.ix());
            return true;
        case Y_STEP:
            _axis.y.stepI(size.ix());
            return true;
        case ORIGIN:
            _origin = size;
            return true;
        case FUNC_TEXT:
        case SIZE:
        default:
            return false;
        }
    }

    void GridLayer::render(RenderContext& canvas)
    {
        canvas.screenGrid(_axis,
                          _majorColor,
                          _minorColor,
                          _originColor,
                          _textColor);
    }

    Vec2F GridLayer::project(const Vec2F& v) const
    {
        const Vec2F extent = scale();

        if (_flags & YUp)
        {
            return {
                extent.x * Half + v.x,
                extent.y - (extent.y * Half + v.y),
            };
        }
        return {(extent.x * Half + v.x),
                (extent.y * Half + v.y)};
    }

    void GridLayer::setAxis(const Axis& ax)
    {
        _axis.set(ax);
    }

    void GridLayer::setFlags(const U8& flags)
    {
        _flags = flags;
    }

}  // namespace Jam::Layers