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
#include "Equation/Symbol.h"
#include "Utils/ParserBase/ParserBase.h"
#include "Utils/String.h"

namespace Jam::Eq
{
    class StmtScanner;
    class CallState;
    using SymbolArray = SimpleArray<Symbol*>;

    class StmtParser final : public ParserBase
    {
    private:
        SymbolArray _symbols;
        I16         _maxDepth{0x80};

        using Parameter = void (StmtParser::*)(CallState& state);

    private:
        void parseImpl(IStream& input) override;
        void writeImpl(OStream& output, int format) override;

        void cleanup();

        Symbol* createSymbol(const int8_t& type);

        String string(const size_t& idx) const;

        String stringToken(const int32_t& idx);

        R64 numericalToken(const int32_t& idx);

        int integer(const size_t& idx) const;

        R64 real(const size_t& idx) const;

        void ruleCsv(CallState& state, const Parameter& r0);

        void ruleFnc(CallState& state);

        void ruleOp3(CallState& state);

        void ruleOp2(CallState& state);

        void ruleOp1(CallState& state);

        void ruleOp(CallState& state);

        void ruleAsn(CallState& state);

        void ruleEq(CallState& state);

        void reset();

    public:
        explicit StmtParser(I16 maxDepth = 0x80);
        ~StmtParser() override;

        const SymbolArray& symbols() const;
    };

}  // namespace Jam::Eq
