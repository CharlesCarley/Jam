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
#include "Equation/Token.h"
#include "Symbol.h"

namespace Jam::Eq
{
    bool ruleFx(const int8_t c)
    {
        switch (c)
        {
        case TOK_ABS:
        case TOK_ACOS:
        case TOK_ASIN:
        case TOK_ATAN:
        case TOK_ATAN2:
        case TOK_CEIL:
        case TOK_COS:
        case TOK_COSH:
        case TOK_EXP:
        case TOK_FABS:
        case TOK_FLOOR:
        case TOK_FMOD:
        case TOK_LOG:
        case TOK_LOG10:
        case TOK_POW:
        case TOK_SIN:
        case TOK_SINH:
        case TOK_SQRT:
        case TOK_TAN:
        case TOK_TANH:
            return true;
        default:
            return false;
        }
    }

    bool isMatchingCloseToken(
        const int8_t a,
        const int8_t b)
    {
        switch (a)
        {
        case TOK_O_PAR:
            return b == TOK_C_PAR;
        case TOK_O_BRACKET:
            return b == TOK_C_BRACKET;
        case TOK_O_BRACE:
            return b == TOK_C_BRACE;
        default:
            return false;
        }
    }

    int8_t mathToken(const int8_t st)
    {
        switch (st)
        {
        case TOK_ABS:
            return MathAbs;
        case TOK_ACOS:
            return MathAcos;
        case TOK_ASIN:
            return MathAsin;
        case TOK_ATAN:
            return MathAtan;
        case TOK_ATAN2:
            return MathAtan2;
        case TOK_CEIL:
            return MathCeil;
        case TOK_COS:
            return MathCos;
        case TOK_COSH:
            return MathCosh;
        case TOK_EXP:
            return MathExp;
        case TOK_FABS:
            return MathFabs;
        case TOK_FLOOR:
            return MathFloor;
        case TOK_FMOD:
            return MathFmod;
        case TOK_LOG:
            return MathLog;
        case TOK_LOG10:
            return MathLog10;
        case TOK_POW:
            return MathPow;
        case TOK_SIN:
            return MathSin;
        case TOK_SINH:
            return MathSinh;
        case TOK_SQRT:
            return MathSqrt;
        case TOK_TAN:
            return MathTan;
        case TOK_TANH:
            return MathTanh;
        case TOK_PI:
            return MathPi;
        //case TOK_E:
        //    return MathE;
        default:
            throw Exception("unknown math token");
        }
    }
}  // namespace Jam::Eq
