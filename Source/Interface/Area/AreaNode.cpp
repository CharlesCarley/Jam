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
#include "Interface/Area/AreaNode.h"
#include "Interface/Extensions.h"

namespace Jam::Editor
{

    AreaNode::AreaNode(AreaCreator* creator, QWidget* parent) :
        QWidget(parent),
        _creator(creator)
    {
        View::emptyWidget(this);
    }

    AreaNode::~AreaNode()
    {
        (void)disconnect(this);
    }

}  // namespace Jam::Editor

#include "AreaNode.Accessors.inl"
#include "AreaNode.Events.inl"
#include "AreaNode.Utilities.inl"
