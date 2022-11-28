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
#include "Math/Vec2.h"
#include "Utils/Array.h"

namespace Jam
{
    class RenderContext;
    class BaseLayer;

    using LayerArray = SimpleArray<BaseLayer*>;

    enum FrameStackCode
    {
        SIZE = 0,
        X_AXIS,
        Y_AXIS,
        X_STEP,
        Y_STEP,
        ORIGIN,
        FUNC_TEXT,
    };

    class FrameStack
    {
    private:
        LayerArray _layers{};

    public:
        FrameStack();
        ~FrameStack();

        void render(RenderContext* canvas);

        bool injectVec2(const FrameStackCode& code,
                        const Vec2F&          value);

        bool injectText(const String&         text);

        // NOTE: passes ownership
        U32 addLayer(BaseLayer* layer);

        BaseLayer* layer(U32 idx);

        const LayerArray& layers() const;

        bool hasLayers() const;

        template <typename T>
        T* cast(const U32 idx);

        void serialize(IStream &data);

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

    inline bool FrameStack::hasLayers() const
    {
        return !_layers.empty();
    }

}  // namespace Jam