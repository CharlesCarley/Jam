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
// <including all headers intentionally>
#include "Utils/Assert.h"
#include "Utils/Allocator.h"
#include "Utils/Array.h"
#include "Utils/ArrayBase.h"
#include "Utils/Char.h"
#include "Utils/Console.h"
#include "Utils/Definitions.h"
#include "Utils/Exception.h"
#include "Utils/FileSystem.h"
#include "Utils/FixedArray.h"
#include "Utils/FixedString.h"
#include "Utils/Hash.h"
#include "Utils/HashMap.h"
#include "Utils/IndexCache.h"
#include "Utils/Path.h"
#include "Utils/Set.h"
#include "Utils/Stack.h"
#include "Utils/String.h"
#include "Utils/TextStreamWriter.h"
#include "Utils/Traits.h"
// </including all headers intentionally>



#if JAM_DEBUG == 1
#include "Utils/Console.h"
#include "Utils/Exception.h"
#endif

namespace Jam
{
#if JAM_DEBUG == 1
    [[noreturn]] void AssertTrap(const char*         declaration,
                                 const char*         file,
                                 const unsigned long line,
                                 const char*         function)
    {
        throw Exception(
            file, '(', line, ')', ": Error assert (", declaration, ") failed!\nIn function", function);
    }

#endif
}  // namespace Jam
