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
#include "Interface/Area/AreaContent.h"
#include "Interface/Area/AreaNode.h"
#include "Interface/Extensions.h"

namespace Jam::Editor
{
    AreaContent::AreaContent(bool type) :
        _type{type}
    {
        View::emptyWidget(this);
        setFocus();
    }

    bool AreaContent::propagate(QEvent* evt)
    {
        return false;
    }

    AreaNode* AreaContent::parentNode() const
    {
        return _parent;
    }

    void AreaContent::setParentNode(AreaNode* parent)
    {
        _parent = parent;
    }
}  // namespace Jam::Editor
