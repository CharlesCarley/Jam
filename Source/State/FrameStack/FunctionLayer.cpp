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
#include "Equation/Stmt.h"
#include "Interface/Areas/OutputArea.h"
#include "Interface/Style/Palette.h"
#include "State/FrameStack/RenderContext.h"

namespace Jam::Layers
{
    using namespace Editor::Const;

    FunctionLayer::FunctionLayer() :
        BaseLayer(FunctionType)
    {
    }

    FunctionLayer::~FunctionLayer() = default;

    void FunctionLayer::render(RenderContext& canvas)
    {
        canvas.selectColor(0xC0C0C0FF);
        canvas.drawVec2F(20, 20, toVec2F(_size), 0);
        canvas.drawAxisF(20, 40, _axis);

        constexpr R32 ss  = 5.f;
        const R32     rss = _size.ry();

        for (I32 i0 = 1; i0 < _size.x; ++i0)
        {
            const Vec2F a = eval(R32(i0 - 1));
            const Vec2F b = eval(R32(i0));

            if (abs(a.y - b.y) <= rss)
            {
                canvas.selectColor(Blue04, 2);
                canvas.drawLine(a.x, a.y, b.x, b.y);
            }
            else
            {
                const Vec2F c = eval(R32(i0 - 1) + Half);

                if (isnan(c.y) && !isnan(b.y))
                {
                    canvas.selectColor(Green04, 2);
                    const R32 yV = sign(a.x) * (_size.ry() + 3);
                    canvas.drawLine(a.x, yV, b.x, b.y);
                }
            }
        }
    }

    Vec2F FunctionLayer::eval(const R32 i0)
    {
        Vec2F p0{i0 - R32(_origin.ix()), 0.f};
        if (!_parser.symbols().empty())
        {
            const Eq::SymbolArray& code = _parser.symbols();

            _stmt.set("x", R64(_axis.x.pointByI(p0.x)));

            const R64 val = _stmt.execute(code);
            p0.y          = _axis.y.pointBy(R32(val));
        }

        p0.x += _origin.x;
        p0.y += _origin.y;
        p0.y = _size.ry() - p0.y;
        return p0;
    }

    bool FunctionLayer::resizeEvent(const Vec2I&)
    {
        _origin.x = _size.rx() * Half;
        _origin.y = _size.ry() * Half;
        return false;
    }

    bool FunctionLayer::injectVec2FImpl(
        const FrameStackCode& code,
        const Vec2F&          size)
    {
        if (code == X_AXIS)
        {
            _axis.set(1, size);
            return true;
        }
        if (code == Y_AXIS)
        {
            _axis.set(0, size);
            return true;
        }
        if (code == X_STEP)
        {
            _axis.x.stepI(I32(size.x));
            return true;
        }
        if (code == Y_STEP)
        {
            _axis.y.stepI(I32(size.x));
            return true;
        }
        if (code == ORIGIN)
        {
            _origin.x = _size.rx() * Half + size.x;
            _origin.y = _size.ry() * Half - size.y;
            return true;
        }
        return false;
    }

    bool FunctionLayer::injectText(const String& text)
    {
        if (_text != text)
        {
            _text = text;
            StringStream ss(_text);

            _parser.read(ss);

            ss.str(String{});
            ss.clear();

            for (const auto sym : _parser.symbols())
            {
                sym->print(ss);
                ss << ' ';
            }
            Editor::Log::writeLine(ss.str());
            return true;
        }
        return false;
    }

}  // namespace Jam::Layers
