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
#include "FrameStackManager.h"
#include <QGuiApplication>
#include <QWidget>
#include "FrameStack/FrameStack.h"
#include "Interface/Events/EventTypes.h"

namespace Jam::Editor::State
{
    FrameStackManager::FrameStackManager()
    {
        _stack = new FrameStack();
    }

    FrameStackManager::~FrameStackManager()
    {
        delete _stack;
        _stack = nullptr;
    }

    void FrameStackManager::clear() const
    {
        if (_stack)
            _stack->clear();
    }

    void FrameStackManager::load(IStream& data) const
    {
        if (_stack)
            _stack->serialize(data);
    }

    void FrameStackManager::addLayer(BaseLayer* layer) const
    {
        if (_stack)
        {
            if (const U32 id = _stack->addLayer(layer);
                id != JtNpos32)
                emit layerAdded(layer->type, id);
        }
    }

    void FrameStackManager::notifyStateChange(const QWidget* widget)
    {
        if (_lock || !widget)
            return;

        // broadcast an event
        _lock = true;
        QGuiApplication::postEvent(
            widget->window(),
            new QEvent((QEvent::Type)(int)LayerUpdate));
    }

    void FrameStackManager::unlock()
    {
        if (_stack)
            _stack->update();
        _lock = false;
    }

    const LayerArray& FrameStackManager::layers() const
    {
        if (!_stack)
            throw Exception("invalid instance");
        return _stack->layers();
    }

    void FrameStackManager::render(const Screen& sc, RenderContext* canvas) const
    {
        if (_stack && canvas)
            _stack->render(sc, canvas);
    }

}  // namespace Jam::Editor::State
