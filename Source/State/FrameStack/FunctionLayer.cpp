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
#include "State/FrameStack/FunctionLayer.h"
#include "Equation/Statement.h"
#include "Interface/Style/Palette.h"
#include "State/FrameStack/RenderContext.h"

namespace Jam::Editor::State
{
    using namespace Const;

    FunctionLayer::FunctionLayer() :
        BaseLayer(FunctionType)
    {
    }

    FunctionLayer::~FunctionLayer()
    {
        for (const auto obj : _array)
            delete obj;
    }

    void FunctionLayer::render(RenderContext& canvas)
    {
        // TODO: remove _size, _axis, and use _screen instead 
        _size = toVec2I(_screen.size());
        _axis = _screen.axis();

        const Vec2F ss = _screen.size() * Half;
        _origin.x = ss.x + _screen.offset().x;
        _origin.y = ss.y - _screen.offset().y;


        canvas.selectColor(0xC0C0C0FF);
        canvas.drawVec2F(20, 20, toVec2F(_size), 0);
        canvas.drawAxisF(20, 40, _axis);
        canvas.drawVec2F(20, 60, _origin);

        for (I32 i0 = 1; i0 < _size.x; ++i0)
            renderExpression(canvas, i0);
    }

    void FunctionLayer::renderExpression(RenderContext& canvas, I32 i0)
    {
        for (const auto obj : _expr)
        {
            const ExpressionStateObject* vso = (ExpressionStateObject*)obj;

            const Vec2F a = eval(R32(i0 - 1), vso->symbols());
            const Vec2F b = eval(R32(i0), vso->symbols());

            if (abs(a.y - b.y) <= _size.ry())
            {
                canvas.selectColor(Blue04, 2);
                canvas.drawLine(a.x, a.y, b.x, b.y);
            }
            else
            {
                const Vec2F c = eval(R32(i0 - 1) + Half, vso->symbols());

                if (isnan(c.y) && !isnan(b.y))
                {
                    canvas.selectColor(Green04, 2);
                    const R32 yV = sign(a.x) * (_size.ry() + 3);
                    canvas.drawLine(a.x, yV, b.x, b.y);
                }
            }
        }
    }

    Vec2F FunctionLayer::eval(const R32 i0, const Eq::SymbolArray& code)
    {
        Vec2F p0{i0 - R32(_origin.ix()), 0.f};
        if (!code.empty())
        {
            _stmt.set("x", R64(_axis.x.pointByI(p0.x)));
            p0.y = _axis.y.pointBy(R32(_stmt.execute(code)));
        }

        p0.x += _origin.x;
        p0.y += _origin.y;
        p0.y = _size.ry() - p0.y;
        return p0;
    }

    bool FunctionLayer::update()
    {
        for (const auto obj : _array)
        {
            if (obj->type() == FstVariable)
            {
                const VariableStateObject* vso = (VariableStateObject*)obj;
                _stmt.set(vso->name(), R64(vso->value()));
            }
        }
        return true;
    }

    VariableStateObject* FunctionLayer::createVariable()
    {
        VariableStateObject* vso = new VariableStateObject();
        _array.push_back(vso);
        return vso;
    }

    ExpressionStateObject* FunctionLayer::createExpression()
    {
        ExpressionStateObject* eso = new ExpressionStateObject();
        _expr.push_back(eso);
        _array.push_back(eso);
        return eso;
    }

    void FunctionLayer::removeVariable(VariableStateObject* vso)
    {
        if (const U32 idx = _array.find(vso); idx != JtNpos32)
            _array.remove(idx);
        delete vso;
    }

    void FunctionLayer::removeExpression(ExpressionStateObject* eso)
    {
        if (const U32 idx = _expr.find(eso); idx != JtNpos32)
            _expr.remove(idx);
        if (const U32 idx = _array.find(eso); idx != JtNpos32)
            _array.remove(idx);
        delete eso;
    }
}  // namespace Jam::Editor::State
