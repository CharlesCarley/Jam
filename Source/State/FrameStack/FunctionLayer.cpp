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
        for (const auto& obj : _memory)
            delete obj.second;
        _memory.clear();
    }

    void FunctionLayer::render(RenderContext& canvas)
    {
        const Vec2F ss = _screen.size() * Half;
        _origin.x      = ss.x + _screen.offset().x;
        _origin.y      = ss.y - _screen.offset().y;

        const I32 w = I32(_screen.w());
        for (I32 i0 = 1; i0 < w; ++i0)
            renderExpression(canvas, i0);
    }

    void FunctionLayer::renderExpression(RenderContext& canvas, const I32 i0)
    {
        for (const auto vso : _expr)
        {
            const Vec2F a = eval(R32(i0 - 1), vso->symbols());
            const Vec2F b = eval(R32(i0), vso->symbols());

            if (abs(a.y - b.y) <= _screen.h())
            {
                canvas.selectColor((QPalette::ColorRole)B03, 2);
                canvas.drawLine(a.x, a.y, b.x, b.y);
            }
            else
            {
                const Vec2F c = eval(R32(i0 - 1) + Half, vso->symbols());

                if (isnan(c.y) && !isnan(b.y))
                {
                    canvas.selectColor((QPalette::ColorRole)R04, 2);
                    const R32 yV = sign(a.x) * (_screen.h() + 3);
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
            const R64 ix = R64(_screen.axis().x.pointByI(p0.x));
            if (_x == JtNpos)
                _stmt.set("x", ix);
            else
                _stmt.set(_x, ix);

            const R64 oy = _stmt.execute(code);

            p0.y = _screen.axis().y.pointBy(R32(oy));
            if (_x == JtNpos)
                _x = _stmt.indexOf("x");

            if (_y == JtNpos)
                _stmt.set("y", oy);
            else
                _stmt.set(_y, oy);
        }

        p0.x += _origin.x;
        p0.y += _origin.y;
        p0.y = _screen.h() - p0.y;
        return p0;
    }

    bool FunctionLayer::update()
    {
        for (const auto vso : _vars)
        {
            if (!vso->name().empty())
                _stmt.set(vso->name(), R64(vso->value()));
        }
        return true;
    }

    VariableStateObject* FunctionLayer::createVariable(const size_t loc)
    {
        VariableStateObject* vso = new VariableStateObject(newRef(), loc);
        _memory.insert(vso->id(), vso);
        _vars.push_back(vso);
        return vso;
    }

    ExpressionStateObject* FunctionLayer::createExpression(const size_t loc)
    {
        ExpressionStateObject* eso = new ExpressionStateObject(newRef(), loc);
        _memory.insert(eso->id(), eso);
        _expr.push_back(eso);
        return eso;
    }

    ExpressionStateObject* FunctionLayer::findExpression(const size_t refId)
    {
        if (const size_t n = _memory.find(refId);
            n != JtNpos)
        {
            if (FunctionStateObject* fso = _memory.at(n);
                fso->type() == FstExpression)
                return (ExpressionStateObject*)fso;
        }
        return nullptr;
    }

    VariableStateObject* FunctionLayer::findVariable(const size_t refId)
    {
        if (const size_t n = _memory.find(refId);
            n != JtNpos)
        {
            if (FunctionStateObject* fso = _memory.at(n);
                fso->type() == FstVariable)
                return (VariableStateObject*)fso;
        }
        return nullptr;
    }

    void FunctionLayer::findStateObjects(const size_t refId, FunctionObjectArray& dest) const
    {
        for (const auto& obj : _memory)
        {
            if (FunctionStateObject* fso = obj.second;
                fso->location() == refId)
                dest.push_back(fso);
        }
    }

    void FunctionLayer::removeVariable(const size_t refId)
    {
        if (VariableStateObject* vso = findVariable(refId))
        {
            if (const U32 idx = _vars.find(vso); idx != JtNpos32)
            {
                _stmt.set(vso->name(), 0);
                _vars.remove(idx);
            }

            _memory.remove(refId);
            delete vso;
        }
    }

    void FunctionLayer::removeExpression(const size_t refId)
    {
        if (ExpressionStateObject* eso = findExpression(refId))
        {
            if (const U32 idx = _expr.find(eso); idx != JtNpos32)
                _expr.remove(idx);
            _memory.remove(refId);
            delete eso;
        }
    }

    size_t FunctionLayer::newRef()
    {
        return ++_ref;
    }

}  // namespace Jam::Editor::State
