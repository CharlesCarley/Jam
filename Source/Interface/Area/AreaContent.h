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
#include "Interface/Area/AreaEdgeRect.h"

namespace Jam::Editor
{
    class AreaNode;

    class AreaContent : public QWidget
    {
        Q_OBJECT
    protected:
        AreaNode* _parent{nullptr};
        bool      _type{false};  // 0 = leaf, 1 = branch

        explicit AreaContent(bool type);


    public:
        AreaNode* parentNode() const;

        void setParentNode(AreaNode* parent);

        bool isBranch() const;

        bool isLeaf() const;

        virtual void updateMask();

        virtual bool propagate(QEvent* evt);
    };

    inline bool AreaContent::isBranch() const
    {
        return _type == true;
    }

    inline bool AreaContent::isLeaf() const
    {
        return _type == false;
    }

    inline void AreaContent::updateMask()
    {
        // empty
    }

}  // namespace Jam::Editor
