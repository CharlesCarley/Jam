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
#include "Equation/Symbol.h"
#include "Math/Lg.h"

namespace Jam::Eq
{
    Symbol::Symbol(const SymbolType tok) :
        _type(tok)
    {
    }

    Symbol::~Symbol() = default;

    void Symbol::print() const
    {
        OutputStringStream oss;
        print(oss);
        Con::write(oss.str());
    }

    void Symbol::print(OStream& out) const
    {
        switch (_type)
        {
        case Numerical:
            out << SetD({_value}, 0);
            break;
        case Identifier:
            out << SetS({_name});
            break;
        case Grouping:
            out << "GR";
            break;
        case Assignment:
            out << "EQ";
            break;
        case Add:
            out << "ADD";
            break;
        case Sub:
            out << "SUB";
            break;
        case Mul:
            out << "MUL";
            break;
        case Div:
            out << "DIV";
            break;
        case Pow:
            out << "POW";
            break;
        case Mod:
            out << "MOD";
            break;
        case Neg:
            out << "NEG";
            break;
        case Not:
            out << "NOT";
            break;
        case BitwiseNot:
            out << "bNot";
            break;
        case MathAbs:
            out << "abs";
            break;
        case MathAcos:
            out << "acos";
            break;
        case MathAsin:
            out << "Asin";
            break;
        case MathAtan:
            out << "Atan";
            break;
        case MathAtan2:
            out << "Atan2";
            break;
        case MathCeil:
            out << "Ceil";
            break;
        case MathCos:
            out << "Cos";
            break;
        case MathCosh:
            out << "Cosh";
            break;
        case MathExp:
            out << "Exp";
            break;
        case MathFabs:
            out << "Fabs";
            break;
        case MathFloor:
            out << "Floor";
            break;
        case MathFmod:
            out << "Fmod";
            break;
        case MathLog:
            out << "Log";
            break;
        case MathLog10:
            out << "Log10";
            break;
        case MathPow:
            out << "Pow";
            break;
        case MathSin:
            out << "Sin";
            break;
        case MathSinh:
            out << "Sinh";
            break;
        case MathSqrt:
            out << "Sqrt";
            break;
        case MathTan:
            out << "Tan";
            break;
        case MathTanh:
            out << "Tanh";
            break;
        case UserFunction:
        case None:
        default:
            break;
        }
        out << " ";
    }

}  // namespace Jam::Eq
