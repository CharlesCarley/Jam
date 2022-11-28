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
#include "Equation/StmtParser.h"
#include "CallState.h"
#include "Equation/StmtScanner.h"
#include "Utils/StreamMethods.h"

namespace Jam::Eq
{

    StmtParser::StmtParser(const I16 maxDepth) :
        _maxDepth(maxDepth)
    {
        _scanner = new StmtScanner();
    }

    StmtParser::~StmtParser()
    {
        for (const auto& symbol : _symbols)
            delete symbol;

        delete _scanner;
        _scanner = nullptr;
    }

    const SymbolArray& StmtParser::symbols() const
    {
        return _symbols;
    }

    void StmtParser::reset()
    {
        for (const auto& symbol : _symbols)
            delete symbol;
        _symbols.resizeFast(0);
        cleanup();
    }

    Symbol* StmtParser::createSymbol(const int8_t& type)
    {
        Symbol* node = new Symbol((SymbolType)type);
        _symbols.push_back(node);
        return node;
    }

    String StmtParser::string(const size_t& idx) const
    {
        return _scanner->string(idx);
    }

    String StmtParser::stringToken(const int32_t& idx)
    {
        return _scanner->string(token(idx).index());
    }

    R64 StmtParser::numericalToken(const int32_t& idx)
    {
        return ((StmtScanner*)_scanner)->real(token(idx).index());
    }

    int StmtParser::integer(const size_t& idx) const
    {
        return _scanner->integer(idx);
    }

    R64 StmtParser::real(const size_t& idx) const
    {
        return ((Eq::StmtScanner*)_scanner)->real(idx);
    }

    void StmtParser::ruleCsv(CallState& state, const Parameter& r0)
    {
        state.depthGuard();

        // <LF> ::= <LF> ',' <r0>
        //         | <r0>
        I8  t0;
        I16 nr = 0;
        do
        {
            (this->*r0)(state);
            t0 = tokenType(0);
            if (t0 == TOK_COMMA)
            {
                nr++;
                advanceCursor();
            }
        } while (t0 == TOK_COMMA);

        state.setCommaCount(nr);
    }

    void StmtParser::ruleFnc(CallState& state)
    {
        state.depthGuard();

        // <Fnc> ::= <Fx> '(' <OpL> ')'
        //         | Id   '(' <OpL> ')'
        const int8_t t0 = tokenType(0);
        const size_t s0 = token(0).index();
        if (tokenType(1) != TOK_O_PAR)
            error("expected an round open bracket.");

        advanceCursor(2);
        ruleCsv(state, &StmtParser::ruleOp);

        if (tokenType(0) != TOK_C_PAR)
            error("expected an round close bracket.");
        advanceCursor();
        if (t0 == TOK_IDENTIFIER)
        {
            // TODO: allow parameter-less methods?
            if (state.commaCount() == 0)
                error("TODO: allow parameter-less methods?");

            createSymbol(Numerical)
                ->setValue(state.commaCount() + 1);
            createSymbol(UserFunction)
                ->setName(string(s0));
        }
        else
        {
            // TODO: Bind function arg count to the math token keyword.
            createSymbol(Numerical)
                ->setValue(state.commaCount() + 1);
            createSymbol(mathToken(t0));
        }
    }

    void StmtParser::ruleOp3(CallState& state)
    {
        state.depthGuard();
        // <Op3> ::= <Fnc>
        //         | '(' <Op> ')'
        //         | Id
        //         | Num

        const int8_t t0 = tokenType(0);
        const int8_t t1 = tokenType(1);

        // <Op3> ::= <Fnc>
        if (t1 == TOK_O_PAR &&
            (t0 == TOK_IDENTIFIER || ruleFx(t0)))
        {
            ruleFnc(state);
            return;
        }

        // <Op3> ::= '(' <Op> ')'
        if (t0 == TOK_O_PAR)
        {
            advanceCursor();
            ruleOp(state);
            if (tokenType(0) != TOK_C_PAR)
                error("expected a group closure");
            advanceCursor();
            return;
        }

        // <Op3> ::= Id
        if (t0 == TOK_IDENTIFIER)
        {
            createSymbol(Identifier)
                ->setName(
                    stringToken(0));
            advanceCursor();
            return;
        }

        // <Op3> ::= Num
        if (isNumericalToken(t0))
        {
            createSymbol(Numerical)
                ->setValue(
                    numericalToken(0));
            advanceCursor();
            return;
        }

        if (t0 == TOK_PI)
        {
            createSymbol(Numerical)
                ->setValue(Pi64);
            advanceCursor();
            return;
        }

        error("unable to deduce a rule from the tokens, ",
              SetI({t0, t1}));
    }

    void StmtParser::ruleOp2(CallState& state)
    {
        state.depthGuard();
        // <Op2>::= <Op2> '*' <Op3>
        //        | <Op2> '/' <Op3>
        //        | <Op2> '%' <Op3>
        //        | <Op2> '^' <Op3>
        //        | <Op3>

        ruleOp3(state);

        switch (tokenType(0))
        {
        // <Op2>::= <Op2> '*' <Op3>
        case TOK_MUL:
            advanceCursor();
            ruleOp2(state);
            createSymbol(Mul);
            break;
        // <Op2>::= <Op2> '/' <Op3>
        case TOK_DIV:
            advanceCursor();
            ruleOp2(state);
            createSymbol(Div);
            break;
        // <Op2>::= <Op2> '%' <Op3>
        case TOK_MOD:
            advanceCursor();
            ruleOp2(state);
            createSymbol(Mod);
            break;
        // <Op2>::= <Op2> '^' <Op3>
        case TOK_POWS:
            advanceCursor();
            ruleOp2(state);
            createSymbol(Pow);
            break;
        default:
            break;
        }
    }

    void StmtParser::ruleOp1(CallState& state)
    {
        state.depthGuard();
        // <Op1> ::= <Op1> '+' <Op2>
        //         | <Op1> '-' <Op2>
        //         | <Op2>

        // <Op1> := <Op2>
        ruleOp2(state);

        switch (tokenType(0))
        {
        // <Op1> ::= <Op1> '+' <Op2>
        case TOK_PLUS:
            advanceCursor();
            ruleOp1(state);
            createSymbol(Add);
            break;
        // <Op1> ::= <Op1> '-' <Op2>
        case TOK_MINUS:
            advanceCursor();
            ruleOp1(state);
            createSymbol(Sub);
            break;
        default:
            break;
        }
    }

    void StmtParser::ruleOp(CallState& state)
    {
        state.depthGuard();
        // <Op> ::= '-' <Op>
        //        | '!' <Op>
        //        | <Op1>
        const int8_t t0 = tokenType(0);

        // <Op> ::= '-' <Op>
        if (t0 == TOK_MINUS)
        {
            advanceCursor();
            ruleOp(state);
            createSymbol(Neg);
            return;
        }

        // <Op> ::= '!' <Op>
        if (t0 == TOK_NOT)
        {
            advanceCursor();
            ruleOp(state);
            createSymbol(Not);
            return;
        }

        // Descending:<Op> ::= <Op1>
        ruleOp1(state);
    }

    void StmtParser::ruleAsn(CallState& state)
    {
        state.depthGuard();
        // <Asn>  ::= Id '=' <SO> <OpL> <SC>
        //          | Id '=' <Asn>
        //          | <Op>
        const int8_t t0 = tokenType(0);
        const int8_t t1 = tokenType(1);
        const int8_t t2 = tokenType(2);

        if (t0 == TOK_IDENTIFIER &&
            t1 == TOK_EQUALS &&
            isOpenToken(t2))
        {
            createSymbol(Identifier)
                ->setName(
                    stringToken(0));
            advanceCursor(3);

            ruleCsv(state, &StmtParser::ruleOp);
            if (!isMatchingCloseToken(t2, tokenType(0)))
            {
                error("invalid matching close token",
                      SetI({tokenType(0)}));
            }

            advanceCursor();
            if (state.commaCount() > 0)
            {
                createSymbol(Numerical)
                    ->setValue(state.commaCount() + 1);
                createSymbol(Grouping);
            }
            createSymbol(Assignment);
            return;
        }

        if (t0 == TOK_IDENTIFIER &&
            t1 == TOK_EQUALS &&
            !isOpenToken(t2))
        {
            createSymbol(Identifier)
                ->setName(
                    stringToken(0));
            advanceCursor(2);
            ruleAsn(state);
            createSymbol(Assignment);
            return;
        }
        ruleOp(state);
    }

    void StmtParser::ruleEq(CallState& state)
    {
        state.resetGuard();
        // <Eq> ::= <Asl>
        //        | <Op>
        //        |
        if (tokenType(1) == TOK_EQUALS)
            ruleCsv(state, &StmtParser::ruleAsn);
        else
            ruleOp(state);
    }

    void StmtParser::parseImpl(IStream& input)
    {
        // make sure the token cursor is at zero
        // initially and attach the input stream
        // to the scanner
        reset();
        _cursor = 0;
        _scanner->attach(&input, PathUtil(_file));

        CallState state = CallState{Clamp<I16>(_maxDepth, 0x10, 0x800)};

        while (_cursor <= (int32_t)_tokens.size())
        {
            // <Eq> ::=
            if (const int8_t tok = token(0).type();
                tok == TOK_EOF)
                break;

            const int32_t op = _cursor;

            // <Eq> ::= <S0>
            ruleEq(state);

            // if the cursor did not advance, force it to.
            if (op == _cursor)
                advanceCursor();
        }
        cleanup();
    }

    void StmtParser::writeImpl(OStream& output, int format)
    {
        // nadda
    }

    void StmtParser::cleanup()
    {
        _tokens.clear();
        if (_scanner)
            ((StmtScanner*)_scanner)->cleanup();
    }
}  // namespace Jam::Eq
