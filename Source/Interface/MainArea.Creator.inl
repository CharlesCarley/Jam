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
#include "Area/AreaCreator.h"
#include "Areas/AreaType.h"
#include "Areas/FrameStackArea.h"
#include "Areas/FunctionArea.h"
#include "Areas/GridArea.h"
#include "Areas/ProjectArea.h"
#include "Areas/TodoArea.h"
#include "Interface/Area/Area.h"
#include "Interface/Areas/OutputArea.h"
#include "Interface/Widgets/IconButton.h"

namespace Jam::Editor
{
    class MainAreaCreator final : public AreaCreator
    {
    public:
        Area* fromType(int32_t type, size_t refId, QWidget* parent) override
        {
            switch (type)
            {
            case AtOutput:
                return new OutputArea(this, refId, parent);
            case AtProject:
                return new ProjectArea(this, refId, parent);
            case AtFrameStack:
                return new FrameStackArea(this, refId, parent);
            case AtFunction:
                return new FunctionArea(this, refId, parent);
            case AtGrid:
                return new GridArea(this, refId, parent);
            case AtMax:
            default:
                return new TodoArea(this, type, refId, parent);
            }
        }

        QString nameFromType(int32_t type) override
        {
            switch (type)
            {
            case AtOutput:
                return "Log";
            case AtProject:
                return "Project";
            case AtFrameStack:
                return "Graph";
            case AtFunction:
                return "Function";
            case AtGrid:
                return "Grid Properties";
            case AtMax:
            default:
                return "Todo";
            }
        }

        Icons::Icon iconFromType(int32_t type) override
        {
            switch (type)
            {
            case AtOutput:
                return Icons::Output;
            case AtProject:
                return Icons::File;
            case AtFrameStack:
                return Icons::GraphArea;
            case AtMax:
            default:
                return Icons::Settings;
            }
        }

        Area* clone(Area* inp) override
        {
            return fromType(inp->type(), JtNpos, nullptr);
        }

        int32_t getNumberOfTypes() override
        {
            return AtMax;
        }
    };

}  // namespace Jam::Editor
