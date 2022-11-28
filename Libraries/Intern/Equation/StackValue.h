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
#include "StmtParser.h"
#include "Utils/Hash.h"
#include "Utils/HashMap.h"
#include "Utils/Stack.h"

namespace Jam
{
    inline hash_t Hash(const String& v)
    {
        return Hash(v.c_str(), v.size());
    }
}  // namespace Jam

namespace Jam::Eq
{
    constexpr U32 InitialHash = 0x3E5;

    struct StackValue
    {
        enum Flags
        {
            Value = 0x0A,
            Id    = 0x0B,
            List  = 0x0C,
        };

        StackValue() = default;

        StackValue(const StackValue& o) = default;

        bool isList() const;
        bool isValue() const;
        bool isId() const;
        I32  integer() const;

        R64    v{0};
        size_t c{JtNpos};
        U8     f{Value};
    };

    using EvalStack     = Stack<StackValue, AOP_SIMPLE_TYPE>;
    using EvalHash      = HashTable<String, StackValue>;
    using ValueGrouping = SimpleArray<StackValue>;
    using EvalGroupHash = HashTable<U32, ValueGrouping*>;
    using ValueList     = SimpleArray<R64>;

    inline bool StackValue::isList() const
    {
        return f == List;
    }

    inline bool StackValue::isValue() const
    {
        return f == Value;
    }

    inline bool StackValue::isId() const
    {
        return f == Id && c != JtNpos;
    }

    inline I32 StackValue::integer() const
    {
        return I32(v);
    }

}  // namespace Jam::Eq
