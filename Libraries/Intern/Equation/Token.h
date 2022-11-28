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
#include "Utils/Char.h"
#include "Utils/ParserBase/TokenBase.h"

namespace Jam::Eq
{
    enum TokenType
    {
        TOK_KW_ST = -27,
        TOK_ABS,
        TOK_ACOS,
        TOK_ASIN,
        TOK_ATAN,
        TOK_ATAN2,
        TOK_CEIL,
        TOK_COS,
        TOK_COSH,
        TOK_EXP,
        TOK_FABS,
        TOK_FLOOR,
        TOK_FMOD,
        TOK_LOG,
        TOK_LOG10,
        TOK_POW,
        TOK_SIN,
        TOK_SINH,
        TOK_SQRT,
        TOK_TAN,
        TOK_TANH,
        //TOK_E,
        TOK_PI,
        TOK_EPSILON,
        TOK_KW_EN,  // MUST == -3
        TOK_EOF = -1,
        TOK_NULL,
        TOK_IDENTIFIER,
        TOK_INT,  // remove
        TOK_FLOAT,
        TOK_EQUALS    = '=',
        TOK_O_BRACKET = '{',
        TOK_C_BRACKET = '}',
        TOK_O_BRACE   = '[',
        TOK_C_BRACE   = ']',
        TOK_O_PAR     = '(',
        TOK_C_PAR     = ')',
        TOK_PLUS      = '+',
        TOK_MINUS     = '-',
        TOK_MUL       = '*',
        TOK_MOD       = '%',
        TOK_DIV       = '/',
        TOK_POWS      = '^',
        TOK_AND       = '&',
        TOK_TILDE     = '~',
        TOK_OR        = '|',
        TOK_PERIOD    = '.',
        TOK_NOT       = '!',
        TOK_COMMA     = ',',
    };

    using Token = TokenBase;

    struct Keyword
    {
        const char* word;
        TokenType   token;
        size_t      max;
    };

    constexpr size_t KeywordMax = TOK_KW_EN - TOK_KW_ST;

    constexpr Keyword Keywords[KeywordMax] = {
        {  "abs",   TOK_ABS, 3},
        { "acos",  TOK_ACOS, 4},
        { "asin",  TOK_ASIN, 4},
        { "atan",  TOK_ATAN, 4},
        {"atan2", TOK_ATAN2, 5},
        { "ceil",  TOK_CEIL, 4},
        { "cosh",  TOK_COSH, 4},
        { "fabs",  TOK_FABS, 4},
        {"floor", TOK_FLOOR, 5},
        { "fmod",  TOK_FMOD, 4},
        {"log10", TOK_LOG10, 5},
        { "sinh",  TOK_SINH, 4},
        { "sqrt",  TOK_SQRT, 4},
        { "tanh",  TOK_TANH, 4},
        {  "cos",   TOK_COS, 3},
        {  "exp",   TOK_EXP, 3},
        {  "log",   TOK_LOG, 3},
        {  "mod",  TOK_FMOD, 3},
        {  "pow",   TOK_POW, 3},
        {  "sin",   TOK_SIN, 3},
        {  "tan",   TOK_TAN, 3},
        {   "pi",    TOK_PI, 2},
        //{    "e",     TOK_E, 1},
    };

    inline bool isOpenToken(const int8_t c)
    {
        return c == TOK_O_PAR || c == TOK_O_BRACKET || c == TOK_O_BRACE;
    }

    inline bool isNumericalToken(const int8_t c)
    {
        return c == TOK_INT || c == TOK_FLOAT;
    }

    extern bool   ruleFx(int8_t c);
    extern int8_t mathToken(int8_t st);
    extern bool   isMatchingCloseToken(int8_t a, int8_t b);

    // [20,2f] : [ !"#%&'()*+,-./]
    // [30,39] : [0...9]
    // [3a,40] : [:;<=>?@]
    // [41,5a] : [A-Z]
    // [3b,60] : [[\]^_`]
    // [61,7a] : [a-z]
    // [7b,7e] : [{|}~]

    inline bool isInFloatSet1(const int ch)
    {
        return ch == '.' ||
               ch == 'E' ||
               ch == 'e';
    }

    inline bool isInFloatSet2(const int ch)
    {
        return ch == '+' ||
               ch == '-' ||
               ch == '.' ||
               ch == 'F' ||
               ch == 'f';
    }

    inline bool isValidCharacter(const int ch)
    {
        if (ch == '"' || ch == '<')
            return false;

        return ch >= ' ' && ch <= 127;
    }

    inline bool isValidIdentifier(const int ch)
    {
        return isLetter(ch) || isDecimal(ch);
    }

    inline bool isQuote(const int ch)
    {
        return ch == '"' || ch == '\'';
    }

}  // namespace Jam::Eq
