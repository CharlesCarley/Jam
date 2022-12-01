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
#include "Math/Screen.h"
#include "Math/Vec2.h"

namespace Jam::Editor::State
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
        const LayerType type;

    protected:
        Vec2I  _size{0, 0};
        Screen _screen;

    public:
        explicit BaseLayer(LayerType type);

        virtual ~BaseLayer() = default;

        virtual void render(RenderContext& canvas);

        virtual bool update();

        void setScreen(const Screen& sc) { _screen = sc; }
    };

}  // namespace Jam::Editor::State
