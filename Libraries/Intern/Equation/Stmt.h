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
#include "Equation/StackValue.h"
#include "Equation/StmtParser.h"

namespace Jam::Eq
{
    typedef  double (*WrapFuncA1)(double a1);
    typedef  double (*WrapFuncA2)(double a1, double a2);

    class Stmt
    {
    private:
        EvalStack     _stack;
        EvalHash      _table;
        EvalGroupHash _groups;
        U32           _hashCount{InitialHash};

        void push(const R64&    v,
                  const size_t& idx  = JtNpos,
                  U8            flag = StackValue::Value);

        void push(const Symbol* sy);

        void store(const Symbol* sym);

        void add();
        void sub();
        void neg();
        void mul();
        void div();
        void mod();
        void pow();
        void group();
        void assign();

        void mathFncA1(WrapFuncA1 f);
        void mathFncA2(WrapFuncA2 f);
        
        void eval(const Symbol* sy);

        R64 executeImpl(const SymbolArray& val);

        template <typename... Args>
        [[noreturn]] void error(Args&&... args);

        [[noreturn]] void argError(const char*);

    public:
        Stmt() = default;
        ~Stmt();

        void set(const String& name, R64 value);
        void set(VInt index, R64 value);

        VInt indexOf(const String& name) const;

        R64 get(const String& name, R64 def = 0);

        R64 peek(I32 idx);

        void get(const String& name, ValueList& dest);

        R64 execute(const SymbolArray& val);
    };

    template <typename... Args>
    void Stmt::error(Args&&... args)
    {
        OutputStringStream stream;
        ((stream << std::forward<Args>(args)), ...);
        throw Exception(stream.str());
    }

}  // namespace Jam::Eq
