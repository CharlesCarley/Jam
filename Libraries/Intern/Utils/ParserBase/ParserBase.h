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
#include <vector>
#include "Utils/ParserBase/ScannerBase.h"
#include "Utils/ParserBase/TokenBase.h"

namespace Jam
{
    class ScannerBase;

    class ParserBase
    {
    public:
        typedef std::vector<TokenBase> Tokens;

    private:
        [[noreturn]] void throwError(const String& message);

        size_t line() const;

    protected:
        int32_t      _cursor{0};
        ScannerBase* _scanner{nullptr};
        Tokens       _tokens;
        String       _file;

        virtual void parseImpl(IStream& is) = 0;

        virtual void writeImpl(OStream& is, int format) = 0;

        TokenBase token(int32_t offs);

        int8_t tokenType(int32_t offs);

        void advanceCursor(int32_t n = 1);

        void readToken(int32_t n = 1);

        template <typename... Args>
        [[noreturn]] void error(
            const String& what,
            Args&&... args);

        virtual void errorMessageImpl(
            String&       dest,
            const String& message);

    public:
        ParserBase()          = default;
        virtual ~ParserBase() = default;

        void read(const String& file);

        void read(IStream& is, const String& file = "");

        void write(const String& file, int format = 0);

        void write(OStream& os, int format = 0);

        String filename() const;
    };


    template <typename... Args>
    [[noreturn]] void ParserBase::error(
        const String& what,
        Args&&... args)
    {
        OutputStringStream stream;
        stream << what;
        ((stream << std::forward<Args>(args)), ...);

        throwError(stream.str());
    }

    inline String ParserBase::filename() const
    {
        return PathUtil(_file).fullPath();
    }

    inline size_t ParserBase::line() const
    {
        if (_scanner)
            return _scanner->line();
        return 0;
    }

}  // namespace Jam
