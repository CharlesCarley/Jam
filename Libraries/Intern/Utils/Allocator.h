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
#include <cstdlib>
#include "Utils/Definitions.h"
#include "Utils/Exception.h"
#include "Utils/Traits.h"

namespace Jam
{
    template <typename Size   = size_t,
              Size UpperBound = MakeLimit<Size>()>
    JAM_FORCE_INLINE constexpr void enforce(Size capacity)
    {
        if (capacity > UpperBound)
        {
            throw Exception(
                "Alloc limit exceed by (",
                (Size)(capacity - UpperBound),
                ") bytes");
        }
    }

    template <typename Type,
              typename Size,
              const Size Limit = MakeLimit<Size>()>
    class AllocBase
    {
    public:
        typedef Type        ValueType;
        typedef Type&       ReferenceType;
        typedef Type*       PointerType;
        typedef const Type* ConstPointerType;
        typedef const Type& ConstReferenceType;
        typedef Size        SizeType;

        static const SizeType npos{Limit};
        static const SizeType limit{Limit};

        void fill(PointerType      dst,
                  ConstPointerType src,
                  const SizeType   cap)
        {
            if (cap > 0 && cap < limit && cap != npos)
                Fill(dst, src, cap);
        }

        void fill(PointerType        dst,
                  ConstReferenceType src,
                  const SizeType     capacity)
        {
            if (capacity > 0 && capacity < limit && capacity != npos)
                Fill<Type>(dst, src, size_t(capacity));
        }

        void copy(const PointerType      dst,
                  const ConstPointerType src,
                  const SizeType         capacity)
        {
            if (capacity > 0 && capacity < limit && capacity != npos)
                Fill<Type>(dst, src, size_t(capacity));
        }

        static void construct(PointerType base, ConstReferenceType v)
        {
            new (base) Type(v);
        }

        static void construct(PointerType base)
        {
            new (base) Type();
        }

        static void destroy(PointerType base)
        {
            if (base)
                base->~Type();
        }

        static void destroy(PointerType beg, PointerType end)
        {
            while (beg && beg != end)
            {
                beg->~Type();
                ++beg;
            }
        }

    protected:
        static void construct(PointerType        beg,
                              ConstPointerType   end,
                              ConstReferenceType value)
        {
            while (beg != end)
            {
                new (beg) Type(value);
                ++beg;
            }
        }

        static void construct(PointerType beg, ConstPointerType end)
        {
            while (beg != end)
            {
                new (beg) Type();
                ++beg;
            }
        }
    };

    template <typename Type,
              typename Size    = size_t,
              const Size Limit = MakeLimit<Size>()>
    class NewAllocator : public AllocBase<Type, Size, Limit>
    {
    public:
        JAM_DECLARE_TYPE(Type)

    public:
        typedef NewAllocator<Type, Size, Limit> SelfType;

    public:
        explicit NewAllocator() = default;

        NewAllocator(const SelfType&) = default;

        ~NewAllocator() = default;

        PointerType allocate()
        {
            Type* ptr = new Type;
            this->construct(ptr);
            return ptr;
        }

        PointerType allocateArray(Size capacity)
        {
            enforce<Size, Limit>(capacity);
            return new Type[capacity];
        }

        PointerType allocateArray(Size capacity, const Type& initial)
        {
            enforce<Size, Limit>(capacity);

            PointerType ptr = new Type[capacity];
            this->fill(ptr, initial, capacity);
            return ptr;
        }

        PointerType reallocateArray(PointerType pointer,
                                    Size        newCap,
                                    Size        oldCap,
                                    const bool  simple = false)
        {
            enforce<Size, Limit>(newCap);

            auto base = new Type[newCap];
            if (pointer)
            {
                if (simple)
                    this->copy(base, pointer, oldCap);
                else
                    this->fill(base, pointer, oldCap);
                delete[] pointer;
            }
            return base;
        }

        static void deallocateArray(
            const ConstPointerType pointer,
            Size)
        {
            delete[] pointer;
        }
    };

    template <typename Type,
              typename Size    = size_t,
              const Size Limit = MakeLimit<Size>()>
    using Allocator = NewAllocator<Type, Size, Limit>;
}  // namespace Jam
