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
#include <QWidget>
#include "Interface/Areas/AreaType.h"
#include "Utils/String.h"

class QLayoutItem;
class QVBoxLayout;

namespace Jam::Editor
{
    class Area;

    class AreaCreator
    {
    public:
        AreaCreator()          = default;
        virtual ~AreaCreator() = default;

        virtual Area*  fromType(int32_t type, QWidget* parent = nullptr) = 0;
        virtual QString nameFromType(int32_t type)                        = 0;
        virtual QIcon  iconFromType(int32_t type)                        = 0;


        virtual int32_t getNumberOfTypes() = 0;

        virtual Area* clone(Area *inp) = 0;
    };

}  // namespace Jam::Editor
