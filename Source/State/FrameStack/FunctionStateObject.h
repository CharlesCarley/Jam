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
#include "Equation/StmtParser.h"
#include "Interface/Events/EventTypes.h"
#include "State/FrameStack/GridLayer.h"

namespace Jam::Editor::State
{
    enum FunctionStateObjectType
    {
        FstNone,
        FstVariable,
        FstExpression
    };

    class FunctionStateObject
    {
    private:
        const I32    _type{FstNone};
        const size_t _id{JtNpos};
        const size_t _loc{JtNpos};

    public:
        explicit FunctionStateObject(const I32 type, const size_t id, const size_t loc) :
            _type(type), _id(id), _loc(loc) {}
        ~FunctionStateObject() = default;

        const I32&    type() const { return _type; }
        const size_t& id() const { return _id; }
        const size_t& location() const { return _loc; }
    };

    using FunctionObjectArray = SimpleArray<FunctionStateObject*>;

    class VariableStateObject : public FunctionStateObject
    {
    private:
        String _name{};
        Vec2F  _range{1.f, 100};
        R32    _rate{1.f};
        R32    _value{1.f};

    public:
        explicit VariableStateObject(const size_t id, const size_t loc) :
            FunctionStateObject(FstVariable, id, loc) {}

        ~VariableStateObject() = default;

        const String& name() const { return _name; }
        const Vec2F&  range() const { return _range; }
        const R32&    rate() const { return _rate; }
        const R32&    value() const { return _value; }

        void setName(const String& val) { _name = val; }
        void setRange(const Vec2F& val) { _range = val; }
        void setRate(const R32& val) { _rate = val; }
        void setValue(const R32& val) { _value = val; }
    };

    class ExpressionStateObject : public FunctionStateObject
    {
    private:
        String         _text{};
        Eq::StmtParser _parser;

    public:
        explicit ExpressionStateObject(const size_t id, const size_t loc) :
            FunctionStateObject(FstExpression, id, loc) {}
        ~ExpressionStateObject() = default;

        const String& text() const { return _text; }

        const Eq::SymbolArray& symbols() const { return _parser.symbols(); }

        void setText(const String& text);
    };
    using Expression = ExpressionStateObject;

}  // namespace Jam::Editor::State