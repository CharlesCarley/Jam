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
#include "FrameStack.h"
#include "Math/Vec2.h"

namespace Jam
{
    class RenderContext;
    enum LayerType
    {
        NoType = 0,
        GridType,
        FunctionType,
    };

    class BaseLayer
    {
    public:
        enum Flags  //[] < sizeof(U8)
        {
            None      = 0x00,
            Selection = 0x01,
        };
        const LayerType type;

    protected:
        Vec2I _size{0, 0};

        virtual bool resizeEvent(const Vec2I& size);

        virtual bool injectVec2FImpl(const FrameStackCode& code,
                                     const Vec2F&          size);

    private:
        U8 _layerFlags{None};

    public:
        explicit BaseLayer(const LayerType type);
        virtual ~BaseLayer() = default;

        virtual void render(RenderContext& canvas);

        virtual bool update();

        bool injectVec2(const FrameStackCode& code,
                        const Vec2F&          size);

        virtual bool injectText(const String& text);

        bool isSelected() const
        {
            return (_layerFlags & Selection) != 0;
        }

        void select(bool v)
        {
            if (v)
                _layerFlags |= Selection;
            else
                _layerFlags &= ~Selection;
        }
    };
}  // namespace Jam
