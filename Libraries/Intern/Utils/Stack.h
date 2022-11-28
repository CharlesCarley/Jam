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
#include "Utils/Array.h"

namespace Jam
{

    template <typename T, uint8_t Options = 0, typename Allocator = Allocator<T, uint32_t> >
    class Stack : public ArrayBase<T, Options, Allocator>
    {
    public:
        JAM_DECLARE_TYPE(T)

        typedef Stack<T, Options, Allocator>     SelfType;
        typedef ArrayBase<T, Options, Allocator> BaseType;
        typedef typename SelfType::SizeType      SizeType;

    private:
        SizeType _first{};
        SizeType _last{};

        SizeType actualSize() const
        {
            JAM_ASSERT(_first <= _last);
            return _last - _first;
        }

    public:
        Stack()               = default;
        Stack(const Stack& q) = default;

        ~Stack()
        {
            clear();
        }

        void clear()
        {
            this->destroy();
        }

        void push(ConstReferenceType value)
        {
            if (this->_size + 1 > this->_capacity)
                this->reserve(this->_size == 0 ? 16 : this->_size * 2);

            if (this->_data)
                this->_data[this->_size++] = value;
        }

        void pushBottom(ConstReferenceType value)
        {
            if (this->_size + 2 > this->_capacity)
            {
                this->reserve(this->_size == 0
                                  ? 16
                                  : this->_size * 2);
            }

            if (this->_data)
            {
                // shift down 1
                const int n = int(this->_size) - 1;

                for (int i = n; i >= 0; --i)
                    Swap(this->_data[i], this->_data[i + 1]);
                this->_data[0] = value;
                ++this->_size;
            }
        }

        void pop()
        {
            if (this->_size > 0)
            {
                if (!(Options & AOP_SIMPLE_TYPE))
                    this->_alloc.destroy(&this->_data[this->_size]);
                --this->_size;
            }
        }

        ConstReferenceType top()
        {
            if (this->_size < 1)
                throw Exception("empty stack");
            return this->_data[this->_size - 1];
        }

        int topI()
        {
            if (this->_size < 1)
                return 0;
            return (int)this->_size - 1;
        }

        ConstReferenceType popTop()
        {
            if (this->_size < 1)
                throw Exception("empty stack");
            return this->_data[--this->_size];
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
    };

}  // namespace Jam
