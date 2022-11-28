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

#include "Utils/Allocator.h"
#include "Utils/ArrayBase.h"

namespace Jam
{
    template <typename T,
              int Options        = AOP_SIMPLE_TYPE,
              typename Allocator = Allocator<T, uint32_t>>
    class Array : public ArrayBase<T, Options, Allocator>
    {
    public:
        JAM_DECLARE_TYPE(T)

        typedef ArrayBase<T, Options, Allocator> BaseType;
        typedef typename Allocator::SizeType     SizeType;

    public:
        Array()               = default;
        Array(const Array& o) = default;
        Array(std::initializer_list<T> o)
        {
            this->reserve(o.size());
            for (const auto& element : o)
                push_back(element);
        }

        explicit Array(const SizeType& initialCapacity) :
            BaseType(initialCapacity)
        {
        }

        Array(const SizeType& initialCapacity, ConstReferenceType fill) :
            BaseType()
        {
            this->resize(initialCapacity, fill);
        }

        ~Array()
        {
            clear();
        }

        void clear()
        {
            this->destroy();
        }

        void push_back(ConstReferenceType v)
        {
            if (this->_size + 1 <= this->_alloc.limit)
            {
                // If the size of the array is known ahead of time
                // and the data is reserved before pushing any elements.
                // The reserved size should be plus one.
                // This is so that the last element will not force
                // an expansion, and it stays within the reserved limit.
                if (this->_size + 1 > this->_capacity)
                {
                    this->reserve(this->_size == 0 ? 16 : this->_size * 2);
                    this->_data[this->_size++] = v;
                }
                else
                    this->_data[this->_size++] = v;
            }
            else
                throw Exception("Allocation limit (", this->_alloc.limit, ") exceed");
        }

        void explicit_push(ConstReferenceType v)
        {
            if (this->_size + 1 <= this->_alloc.limit)
            {
                if (this->_data != nullptr && this->_size + 1 <= this->_capacity)
                    this->_data[this->_size++] = v;
                else
                    throw Exception("push overflow");
            }
            else
                throw Exception("Allocation limit (", this->_alloc.limit, ") exceed");
        }

        void pop_back()
        {
            if (this->_size > 0)
            {
                if (!(Options & AOP_SIMPLE_TYPE))
                    this->_alloc.destroy(&this->_data[this->_size]);
                --this->_size;
            }
        }

        void erase(ConstReferenceType v)
        {
            remove(this->find(v));
        }

        void remove(SizeType pos)
        {
            // This is a fast unordered remove.
            // The downside is that any creation order is lost.
            if (this->_size > 0)
            {
                if (pos < this->_capacity)
                {
                    Swap(this->_data[pos],
                         this->_data[this->_size - 1]);

                    if (!(Options & AOP_SIMPLE_TYPE))
                        this->_alloc.destroy(&this->_data[--this->_size]);
                    else
                        --this->_size;
                }
            }
        }

        void removeOrdered(SizeType pos)
        {
            // This is a slow, ordered remove. The only upside is that
            // the original order is preserved.
            if (this->_size > 0)
            {
                if (pos < this->_capacity)
                {
                    // shift the element to the end
                    for (SizeType i = pos + 1; i < this->_size; ++i)
                    {
                        Swap(this->_data[i - 1],
                             this->_data[i]);
                    }

                    if (!(Options & AOP_SIMPLE_TYPE))
                        this->_alloc.destroy(&this->_data[--this->_size]);
                    else
                        --this->_size;
                }
            }
        }

        ReferenceType operator[](SizeType idx)
        {
            JAM_ASSERT(idx < this->_capacity)
            return this->_data[idx];
        }

        ConstReferenceType operator[](SizeType idx) const
        {
            JAM_ASSERT(idx < this->_capacity)
            return this->_data[idx];
        }

        ReferenceType at(SizeType idx)
        {
            JAM_ASSERT(idx < this->_capacity)
            return this->_data[idx];
        }

        ConstReferenceType at(SizeType idx) const
        {
            JAM_ASSERT(idx < this->_capacity)
            return this->_data[idx];
        }

        ReferenceType front()
        {
            JAM_ASSERT(this->_size > 0)
            return this->_data[0];
        }

        ReferenceType back()
        {
            JAM_ASSERT(this->_size > 0)
            return this->_data[this->_size - 1];
        }

        ConstReferenceType front() const
        {
            JAM_ASSERT(this->_size > 0)
            return this->_data[0];
        }

        ConstReferenceType back() const
        {
            JAM_ASSERT(this->_size > 0)
            return this->_data[this->_size - 1];
        }

        PointerType begin()
        {
            if (this->_size > 0)
                return &this->_data[0];
            return nullptr;
        }

        PointerType end()
        {
            if (this->_size > 0)
                return &this->_data[this->_size];
            return nullptr;
        }

        ConstPointerType begin() const
        {
            if (this->_size > 0)
                return &this->_data[0];
            return nullptr;
        }

        ConstPointerType end() const
        {
            if (this->_size > 0)
                return &this->_data[this->_size];
            return nullptr;
        }

        Array& operator=(const Array& rhs)
        {
            this->replicate(rhs);
            return *this;
        }
    };

    template <typename T, typename Allocator = Allocator<T, uint32_t>>
    class SimpleArray : public Array<T, AOP_SIMPLE_TYPE, Allocator>
    {
    public:
        SimpleArray()                     = default;
        SimpleArray(const SimpleArray& o) = default;

        SimpleArray(std::initializer_list<T> o) :
            Array(o)
        {
        }
    };

}  // namespace Jam
