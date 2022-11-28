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

#include <cstdint>

namespace Jam
{
    using hash_t = size_t;

    extern hash_t Hash(const char* key);
    extern hash_t Hash(const char* key, size_t len);
    extern hash_t Hash(const uint32_t& key);
    extern hash_t Hash(const uint64_t& key);
    extern hash_t Hash(const void* key);

    extern void NextPow2(size_t& x);

    // https://graphics.stanford.edu/~seander/bithacks.html#DetermineIfPowerOf2
    inline bool IsPow2(const size_t& x)
    {
        return x && (x & (x - 1)) == 0;
    }

}  // namespace Jam
