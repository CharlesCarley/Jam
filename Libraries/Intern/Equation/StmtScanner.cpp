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
#include "Equation/StmtScanner.h"
#include "Equation/Token.h"
#include "Utils/Char.h"

namespace Jam::Eq
{
    StmtScanner::StmtScanner() = default;

    void StmtScanner::cleanup()
    {
        _doubles.clear();
        ScannerBase::cleanup();
    }

    TokenType filterKeyword(const ScratchBuffer& input)
    {
        const char*  test = input.data();
        const size_t al   = input.size()-1;
        for (const auto& [word, token, bl] : Keywords)
        {
            if (Char::equals(test, al, word, bl))
                return token;
        }
        return TOK_NULL;
    }

    void StmtScanner::scanIdentifier(Token& tok)
    {
        int ch = _stream->peek();
        if (!isLetter(ch))
            syntaxError("expected the quote character '\"'");

        _buf.resizeFast(0);

        ch = _stream->get();
        while (isValidIdentifier(ch))
        {
            _buf.push_back((char)ch);
            ch = _stream->get();
        }
        _buf.push_back(0x00);

        if (ch > 0)
            _stream->putback((char)ch);

        if (const TokenType rt = filterKeyword(_buf);
            rt != TOK_NULL)
        {
            tok.setType(rt);
        }
        else
        {
            tok.setIndex(
                ScannerBase::save(
                    {
                        _buf.data(),
                    })
            );
            tok.setType(TOK_IDENTIFIER);
        }
    }

    void StmtScanner::scanNumber(Token& tok)
    {
        int ch = _stream->peek();
        if (!isDecimal(ch))
            syntaxError("expected a decimal");

        bool hasExtra = false;

        _buf.resizeFast(0);
        ch = _stream->get();

        while (isDecimal(ch) ||
               (!hasExtra && isInFloatSet1(ch)) ||
               (hasExtra && isInFloatSet2(ch)))
        {
            if (ch == 'E' || ch == 'e')
                hasExtra = true;

            _buf.push_back(int8_t(ch));

            ch = _stream->get();
        }

        if (ch > 0)
        {
            // put back the symbol that
            // tested not to be a digit.
            _stream->putback((char)ch);
        }

        if (_buf.sizeI() > 0)
        {
            _buf.push_back('\0');
            const double v = Char::toDouble(_buf.data());
            _buf.resizeFast(0);

            tok.setIndex(save(v));
            tok.setType(TOK_FLOAT);
        }
        else
        {
            syntaxError("WTH, expected to parse a double");
        }
    }

    void StmtScanner::scan(Token& tok)
    {
        if (_stream == nullptr)
            syntaxError("No supplied stream");

        tok.clear();

        int ch;
        while ((ch = _stream->get()) > 0)
        {
            tok.setLine(_line);

            switch (ch)
            {
            case UpperCaseAz:
            case LowerCaseAz:
                _stream->putback((char)ch);
                scanIdentifier(tok);
                return;
            case Digits09:
                _stream->putback((char)ch);
                scanNumber(tok);
                return;
            case '=':
                tok.setType(TOK_EQUALS);
                return;
            case '{':
                tok.setType(TOK_O_BRACKET);
                return;
            case '}':
                tok.setType(TOK_C_BRACKET);
                return;
            case '[':
                tok.setType(TOK_O_BRACE);
                return;
            case ']':
                tok.setType(TOK_C_BRACE);
                return;
            case '(':
                tok.setType(TOK_O_PAR);
                return;
            case ')':
                tok.setType(TOK_C_PAR);
                return;
            case '+':
                tok.setType(TOK_PLUS);
                return;
            case '-':
                tok.setType(TOK_MINUS);
                return;
            case '*':
                tok.setType(TOK_MUL);
                return;
            case '/':
                tok.setType(TOK_DIV);
                return;
            case '^':
                tok.setType(TOK_POWS);
                return;
            case '.':
                tok.setType(TOK_PERIOD);
                return;
            case '!':
                tok.setType(TOK_NOT);
                return;
            case ',':
                tok.setType(TOK_COMMA);
                return;
            case '%':
                tok.setType(TOK_MOD);
                break;
            case '&':
                tok.setType(TOK_AND);
                break;
            case '~':
                tok.setType(TOK_TILDE);
                break;
            case '|':
                tok.setType(TOK_OR);
                break;
            case '\r':
            case '\n':
                _line++;
                break;
            case '\t':
            case ' ':
                break;
            case '#':
                scanLineComment();
                break;
            default:
                syntaxError(
                    "unknown character parsed 0x",
                    Char::toHexString((uint8_t)ch),
                    "'");
            }
        }
        tok.setType(TOK_EOF);
    }

    double StmtScanner::real(const size_t& idx, const double def) const
    {
        if (idx < _doubles.size())
            return _doubles.at(idx);
        return def;
    }
}  // namespace Jam::Eq
