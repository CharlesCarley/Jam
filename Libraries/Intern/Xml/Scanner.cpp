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
#include "Xml/Scanner.h"
#include "Utils/Char.h"
#include "Xml/Token.h"

namespace Jam::Xml
{
    Scanner::Scanner() :
        _defaultState(true)
    {
    }

    inline bool isValidCharacter(const int ch)
    {
        if (ch == '"' || ch == '<')
            return false;

        return ch >= ' ' && ch <= 127;
    }

    inline bool isValidIdentifier(const int ch)
    {
        return isLetter(ch) || isDecimal(ch) || ch == '_' || ch == ':';
    }

    inline bool isQuote(const int ch)
    {
        return ch == '"' || ch == '\'';
    }

    void Scanner::getCode(String& dest, const size_t& idx)
    {
        if (idx < _code.size())
            dest = _code.at(idx);
        else
            syntaxError("code index out of bounds");
    }

    void Scanner::scanString(Token& tok)
    {
        int ch = _stream->get();
        if (!isQuote(ch))
            syntaxError("expected the quote character '\"'");

        String dest;
        ch = _stream->get();
        while (!isQuote(ch))
        {
            dest.push_back((char)ch);
            ch = _stream->get();

            if (ch <= 0)
                syntaxError("unexpected end of file");
        }

        tok.setIndex(save(dest));
        tok.setType(TOK_STRING);
    }

    void Scanner::scanSymbol(Token& tok)
    {
        int ch = _stream->get();

        if (isLetter(ch))
        {
            String cmp;
            while (isValidIdentifier(ch))
            {
                cmp.push_back((char)ch);
                ch = _stream->get();
            }

            _stream->putback((char)ch);

            if (cmp == "xml")
                tok.setType(TOK_KW_XML);
            else
            {
                // If it's not a reserved word
                // save it as an identifier.

                tok.setType(TOK_IDENTIFIER);
                tok.setIndex(save(cmp));
            }
        }
    }

    void Scanner::scan(Token& tok)
    {
        if (_stream == nullptr)
            syntaxError("No supplied stream");

        tok.clear();

        int ch;
        while ((ch = _stream->get()) > 0)
        {
            tok.setLine(_line);
            if (_defaultState)
            {
                switch (ch)
                {
                case '<':
                    if (_stream->peek() == '!')
                        scanMultiLineComment();
                    else
                    {
                        tok.setType(TOK_ST_TAG);
                        return;
                    }
                    break;
                case '\'':
                case '"':
                    _stream->putback((char)ch);
                    scanString(tok);
                    return;
                case '=':
                    tok.setType(TOK_EQUALS);
                    return;
                case '>':
                    _defaultState = false;
                    tok.setType(TOK_EN_TAG);
                    return;
                case '/':
                    tok.setType(TOK_SLASH);
                    return;
                case '?':
                    tok.setType(TOK_QUESTION);
                    return;
                case ':':
                case Digits09:
                case LowerCaseAz:
                case UpperCaseAz:
                    _stream->putback((char)ch);
                    scanSymbol(tok);
                    return;
                case '\r':
                case '\n':
                    if (ch == '\r' && _stream->peek() == '\n')
                        _stream->get();
                    ++_line;
                    break;
                case ' ':
                case '\t':
                    scanWhiteSpace();
                    break;
                default:
                    syntaxError("unknown character parsed #x", Char::toHexString((uint8_t)ch), "'");
                }
            }
            else
            {
                OutputStringStream oss;

                bool onlyWhiteSpace = true;
                while (ch != '<')
                {
                    oss << (char)ch;

                    if (onlyWhiteSpace)
                        onlyWhiteSpace = isWhiteSpace(ch);

                    ch = _stream->get();

                    if (ch <= 0)
                        break;
                }

                _stream->putback((char)ch);

                String dest = oss.str();

                _defaultState = true;

                if (!dest.empty() && !onlyWhiteSpace)
                {
                    tok.setIndex(_code.size());
                    _code.push_back(dest);
                    tok.setType(TOK_TEXT);
                    return;
                }
            }
        }

        tok.setType(TOK_EOF);
    }
}  // namespace Jam::Xml
