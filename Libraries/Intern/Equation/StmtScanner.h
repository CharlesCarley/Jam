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
#include "Equation/Token.h"
#include "Utils/Array.h"
#include "Utils/ParserBase/ScannerBase.h"

namespace Jam::Eq
{
    using DoubleTable   = IndexCache<double>;
    using ScratchBuffer = SimpleArray<char>;

    class StmtScanner final : public ScannerBase
    {
    private:
        DoubleTable   _doubles;
        ScratchBuffer _buf;

        void scanNumber(Token& tok);

        void scanIdentifier(Token& tok);

        size_t save(const double& val)
        {
            return _doubles.insert(val);
        }

    public:
        StmtScanner();

        void cleanup() override;

        void   scan(Token& tok) override;
        double real(const size_t& idx, double def = 0.0) const;
    };
}  // namespace Jam::Eq
