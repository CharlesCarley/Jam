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
#include <QObject>
#include "FrameStack/BaseLayer.h"
#include "FrameStack/FrameStack.h"

namespace Jam::Editor::State
{

    class FrameStackManager final : public QObject
    {
    public:
        Q_OBJECT

    signals:
        void layerAdded(const I32& type, const U32& index) const;

    private:
        // This is a friend so that only ApplicationState may
        // instance this class. (do not access class members
        // with the extra freedom)
        friend class App;

        FrameStack* _stack{nullptr};

        bool _lock{false};

        FrameStackManager();

        ~FrameStackManager() override;

    public:
        FrameStack* stack() const;

        void clear() const;

        void render(const Screen& sc, RenderContext* canvas) const;

        void addLayer(BaseLayer* layer) const;

        const LayerArray& layers() const;

        void load(IStream& data) const;

        void save(OStream& data) const;

        void notifyStateChange(const QWidget* widget);

        void unlock();

        template <typename T, I32 Type>
        T* cast(U32 idx);
    };

    inline void FrameStackManager::unlock()
    {
        _lock = false;
    }

    inline FrameStack* FrameStackManager::stack() const
    {
        return _stack;
    }

    template <typename T, I32 Type>
    T* FrameStackManager::cast(const U32 idx)
    {
        if (_stack)
        {
            if (BaseLayer* lay = _stack->layer(idx);
                lay && lay->type == Type)
                return (T*)lay;
        }
        throw Exception("Invalid layer cast");
    }

}  // namespace Jam::Editor::State
