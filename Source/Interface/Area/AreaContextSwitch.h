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
#include <QPushButton>
#include "Interface/Areas/AreaType.h"

namespace Jam::Editor
{
    class AreaCreator;

    class AreaContextSwitch final : public QPushButton
    {
        Q_OBJECT
    signals:
        void wantsContextSwitch(int type);

    protected:
        void mousePressEvent(QMouseEvent* e) override;

    private:
        AreaCreator* _creator{nullptr};
        int32_t      _type{-1};

        void construct();

    public:
        explicit AreaContextSwitch(AreaCreator* creator,
                                   int32_t      type,
                                   QWidget*     parent = nullptr);
    };

}  // namespace Jam::Editor
