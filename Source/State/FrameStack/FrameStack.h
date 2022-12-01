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
#include "Math/Screen.h"
#include "Math/Vec2.h"
#include "Utils/Array.h"

namespace Jam::Editor::State
{
    class RenderContext;
    class BaseLayer;

    using LayerArray = SimpleArray<BaseLayer*>;

    class FrameStack
    {
    private:
        LayerArray _layers{};

    public:
        FrameStack();
        ~FrameStack();

        void render(const Screen& sc, RenderContext* canvas);

        void update();

        // NOTE: passes ownership
        U32 addLayer(BaseLayer* layer);

        BaseLayer* layer(U32 idx);

        const LayerArray& layers() const;

        template <typename T>
        T* cast(U32 idx);

        void serialize(IStream& data);

        void clear();
    };

    template <typename T>
    T* FrameStack::cast(const U32 idx)
    {
        return (T*)layer(idx);
    }

    inline const LayerArray& FrameStack::layers() const
    {
        return _layers;
    }

}  // namespace Jam::Editor::State