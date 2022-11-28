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

#include "Utils/String.h"

namespace Jam::CommandLine
{
    enum TokenType
    {
        TOK_NONE,
        TOK_SWITCH_SHORT,
        TOK_SWITCH_LONG,
        TOK_NEXT,
        TOK_OPTION,
        // delete
        TOK_INTEGER,
        // delete
        TOK_REAL,
        // delete
        TOK_IDENTIFIER,
        // rename to value
        TOK_FILEPATH,
        TOK_EOS,
        TOK_ERROR
    };

    class Token
    {
    private:
        int    _type;
        String _value;

    public:
        Token() :
            _type(TOK_NONE)
        {
            _value.reserve(64);
        }

        const int& getType() const
        {
            return _type;
        }

        void setType(const int& type)
        {
            _type = type;
        }

        const String& getValue() const
        {
            return _value;
        }

        void append(const char& ch)
        {
            _value.push_back(ch);
        }

        void append(const String& str)
        {
            _value.append(str);
        }

        void clear()
        {
            _type = TOK_EOS;
            _value.resize(0);
        }
    };
}  // namespace Jam::CommandLine
