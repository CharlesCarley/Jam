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
#include "BaseLayer.h"
#include "Equation/Statement.h"
#include "Equation/StmtParser.h"
#include "FunctionStateObject.h"
#include "State/FrameStack/GridLayer.h"

namespace Jam::Editor::State
{
    class FunctionLayer final : public BaseLayer
    {
    private:
        Vec2F               _origin{0.f, 0.f};
        Eq::Statement       _stmt;
        FunctionObjectArray _array;
        FunctionObjectArray _expr;
        VInt                _x{JtNpos};
        // Todo: allow y step. VInt _y{JtNpos};

    public:
        FunctionLayer();
        ~FunctionLayer() override;

        VariableStateObject*   createVariable();
        ExpressionStateObject* createExpression();

        void removeVariable(VariableStateObject* vso);
        void removeExpression(ExpressionStateObject* eso);

        const FunctionObjectArray& objects() const;

    private:
        Vec2F eval(R32 i0, const Eq::SymbolArray& code);

        void render(RenderContext& canvas) override;

        bool update() override;

        void renderExpression(RenderContext& canvas, I32 i0);
    };

    inline const FunctionObjectArray& FunctionLayer::objects() const
    {
        return _array;
    }

}  // namespace Jam::Editor::State