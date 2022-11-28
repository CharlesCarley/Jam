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
#include "Utils/Hash.h"
#include "Utils/Char.h"
#include "Utils/Definitions.h"

namespace Jam
{
    // magic numbers from http://www.isthe.com/chongo/tech/comp/fnv/
    // constexpr size_t InitialFnv2 = 0x9E3779B9;

    constexpr size_t InitialFnv  = 0x9E3779B1;
    constexpr size_t MultipleFnv = 0x1000193;

    hash_t Hash(const char* key)
    {
        if (!key)
            return JtNpos;
        return Hash(key, Char::length(key));
    }

    // Fowler/Noll/Vo (FNV) Hash
    hash_t Hash(const char* key, const size_t len)
    {
        if (!key || len == 0 || len == JtNpos)
            return JtNpos;

        size_t hash = InitialFnv;

        for (size_t i = 0; key[i] && i < len; i++)
        {
            hash = hash ^ key[i];       // xor the low 8 bits
            hash = hash * MultipleFnv;  // multiply by the magic number
        }
        return hash;
    }

    hash_t Hash(const uint32_t& key)
    {
        hash_t hash = (hash_t)key * InitialFnv;
        hash += ~(hash << 15);
        hash ^= hash >> 10;
        hash += hash << 3;
        hash ^= hash >> 6;
        hash += ~(hash << 11);
        hash ^= hash >> 16;
        return hash;
    }

    hash_t Hash(const void* key)
    {
        hash_t hash = (uintptr_t)key * InitialFnv;
        hash += ~(hash << 15);
        hash ^= hash >> 10;
        hash += hash << 3;
        hash ^= hash >> 6;
        hash += ~(hash << 11);
        hash ^= hash >> 16;
        return hash;
    }

    hash_t Hash(const uint64_t& key)
    {
        return Hash((void*)key);
    }

    // https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
    void NextPow2(size_t& x)
    {
        --x;
        x |= x >> 16;
        x |= x >> 8;
        x |= x >> 4;
        x |= x >> 2;
        x |= x >> 1;
        ++x;
    }

}  // namespace Jam
