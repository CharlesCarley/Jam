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
#include "BaseLayer.h"
#include "Math/Axis.h"
#include "Math/Real.h"
#include "Math/Vec2F.h"

namespace Jam::Layers
{
    class GridLayer final : public BaseLayer
    {
    public:
        enum Flags
        {
            YUp = 0x01
        };

    protected:
        bool resizeEvent(const Vec2I& oldSize) override;

    private:
        U32   _majorColor{0x2b2b2bFF};
        U32   _minorColor{0x212121FF};
        U32   _originColor{0x4B4B4BFF};
        U32   _textColor{0xC0C0CFFF};
        Vec2F _origin{U32(0), 0};
        Axis  _axis;
        U8    _flags{None};

        Vec2F scale() const;

        bool injectVec2FImpl(const FrameStackCode& code,
                             const Vec2F&          size) override;

    public:
        GridLayer();
        ~GridLayer() override = default;

        void render(RenderContext& canvas) override;

        Vec2F project(const Vec2F& v) const;

        void setAxis(const Axis& ax);

        void setMajorGrid(const U32& color);
        void setMinorGrid(const U32& color);
        void setOrigin(const U32& color);

        void setFlags(const U8& flags);

        void setOrigin(const Vec2F& origin);

        const Axis& axis() const { return _axis; }

        const Vec2F& origin() const { return _origin; }
    };

    inline Vec2F GridLayer::scale() const
    {
        return {R32(_size.x), R32(_size.y)};
    }

    inline void GridLayer::setMajorGrid(const U32& color)
    {
        _majorColor = color;
    }

    inline void GridLayer::setMinorGrid(const U32& color)
    {
        _minorColor = color;
    }

    inline void GridLayer::setOrigin(const U32& color)
    {
        _originColor = color;
    }

    inline void GridLayer::setOrigin(const Vec2F& origin)
    {
        _origin = origin;
    }

}  // namespace Jam::Layers