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
#include "State/FrameStack/FrameStack.h"
#include "FunctionLayer.h"
#include "State/FrameStack/FrameStackSerialize.h"
#include "State/FrameStack/BaseLayer.h"

namespace Jam
{
    FrameStack::FrameStack() = default;

    FrameStack::~FrameStack()
    {
        clear();
    }

    void FrameStack::render(RenderContext* canvas)
    {
        for (BaseLayer* element : _layers)
            element->render(*canvas);
    }

    bool FrameStack::injectVec2(
        const FrameStackCode& code,
        const Vec2F&          value)
    {
        bool mod = false;
        for (BaseLayer* element : _layers)
            mod = element->injectVec2(code, value) || mod;
        return mod;
    }

    bool FrameStack::injectText(
        const String& text)
    {
        bool mod = false;
        for (BaseLayer* element : _layers)
            mod = element->injectText(text) || mod;
        return mod;
    }

    U32 FrameStack::addLayer(BaseLayer* layer)
    {
        U32 loc = JtNpos32;
        if (layer)
        {
            loc = _layers.size();
            _layers.push_back(layer);
        }
        return loc;
    }

    BaseLayer* FrameStack::layer(const U32 idx)
    {
        if (idx < _layers.size())
            return _layers.at(idx);
        return nullptr;
    }

    void FrameStack::serialize(IStream &data)
    {
        const FrameStackSerialize serialize(this);
        serialize.load(data);
    }

    void FrameStack::clear()
    {
        for (const BaseLayer* element : _layers)
            delete element;
        _layers.clear();
    }
}  // namespace Jam