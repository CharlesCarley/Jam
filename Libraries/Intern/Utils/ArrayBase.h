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
#include "Utils/Definitions.h"

namespace Jam
{
    enum ArrayOptions
    {
        AOP_DEFAULT_TYPE = 0x01,
        // Defining the option as a simple type,
        // means that the array is storing an atomic or
        // pointer type. When resizing, reserving,
        // or clearing the buffer the ~T destructor is
        // not called. In some cases this allows bypassing
        // A loop of N elements. This will not work for
        // stack based objects that require copy constructors
        // or have memory that needs deallocated in a destructor.
        AOP_SIMPLE_TYPE = 0x02,
        AOP_EXPAND_MUL2 = 0x04,
    };

    /**
     * \brief Provides general functionality for array based data structures.
     */
    template <typename T,
              uint8_t Options    = 0,
              typename Allocator = Allocator<T, uint32_t, MakeLimit<uint16_t>()>>
    class ArrayBase
    {
    public:
        JAM_DECLARE_TYPE(T)

        /**
         * \brief Defines shorthand access to the current template type. 
         */
        using SelfType = ArrayBase<T, Options, Allocator>;

        /**
         * \brief Defines shorthand access to the supplied size type.
         */
        using SizeType = typename Allocator::SizeType;

    protected:
        PointerType _data{nullptr};
        SizeType    _size{0};
        SizeType    _capacity{0};
        Allocator   _alloc;

    public:
        SizeType find(ConstReferenceType v) const
        {
            SizeType i;
            for (i = 0; i < _size; ++i)
            {
                if (_data[i] == v)
                    return i;
            }
            return Allocator::npos;
        }

        // This assumes that the array was previously sorted by the 'Param' type.
        // Param should be some common member of the type T.
        // Returns NPOS if not found or the index of the found array element.
        template <typename Param>
        SizeType findBinary(int (*cmpFunc)(ValueType a, Param b), Param param) const
        {
            if (!cmpFunc || _size <= 0 || !_data)
                return Allocator::npos;

            SizeType f = 0, l = _size - 1;
            while (f <= l)
            {
                const SizeType m = (f + l) >> 1;

                const int c = cmpFunc(_data[m], param);
                if (c == 0)
                    return m;
                if (c > 0)
                    l = m - 1;
                else
                    f = m + 1;
            }
            return Allocator::npos;
        }

        SizeType findBinary(ConstReferenceType key) const
        {
            if (_size <= 0 || !_data)
                return Allocator::npos;

            SizeType f = 0, l = _size - 1;
            while (f <= l)
            {
                const SizeType m = (f + l) >> 1;
                if (_data[m] == key)
                    return m;
                if (_data[m] > key)
                    l = m - 1;
                else
                    f = m + 1;
            }
            return Allocator::npos;
        }

        // Do not call this with a type T that has
        // cleanup code in ~T(). This is intended
        // for atomic and simple class types.
        void resizeFast(SizeType nr)
        {
            if (nr > _size)
                reserve(nr);
            _size = nr;
        }

        void resize(SizeType nr)
        {
            if (nr < _size)
            {
                if (!(Options & AOP_SIMPLE_TYPE))
                {
                    for (SizeType i = nr; i < _size; ++i)
                        _alloc.destroy(&_data[i]);
                }
            }
            else
            {
                if (nr > _size)
                    reserve(nr);
            }
            _size = nr;
        }

        void resize(SizeType nr, ConstReferenceType fill)
        {
            SizeType i;

            if (nr < _size)
            {
                if (!(Options & AOP_SIMPLE_TYPE))
                {
                    for (i = nr; i < _size; ++i)
                        _data[i].~T();
                }
            }
            else
            {
                if (nr > _size)
                    reserve(nr);
                _alloc.fill(_data + _size, fill, nr);
            }
            _size = nr;
        }

        // This is using capacity plus one in order to reserve
        // and then push up to the max without expanding the
        // data. One more element after that will cause the expansion.
        void reserve(SizeType capacity)
        {
            explicit_reserve(capacity + 1);
        }

        void explicit_reserve(SizeType capacity)
        {
            if (_capacity < capacity)
            {
                capacity = Min<uint32_t>(capacity, _alloc.limit);
                if (_data)
                {
                    _data     = _alloc.reallocateArray(_data,
                                                   capacity,
                                                   _size,
                                                   (Options & AOP_SIMPLE_TYPE) != 0);
                    _capacity = capacity;
                }
                else
                {
                    _data     = _alloc.allocateArray(capacity);
                    _capacity = capacity;
                }

                if (!_data)
                    throw Exception("Failed to reserve array memory");
            }
        }

        void write(ConstPointerType writeData, SizeType writeDataCount)
        {
            if (_capacity < writeDataCount)
                reserve(writeDataCount);

            if (Options & AOP_SIMPLE_TYPE)  // does not need T()
                ::memcpy(_data, writeData, writeDataCount);
            else
                _alloc.fill(_data, writeData, writeDataCount);

            _size = writeDataCount;
        }

        ConstPointerType data() const
        {
            return _data;
        }

        PointerType data()
        {
            return _data;
        }

        bool valid() const
        {
            return _data != nullptr;
        }

        SizeType capacity() const
        {
            return _capacity;
        }

        SizeType size() const
        {
            return _size;
        }

        int sizeI() const
        {
            return Clamp<int>(_size, 0, 0x7FFFFFFF);
        }

        bool empty() const
        {
            return _size < 1;
        }

        bool isNotEmpty() const
        {
            return _size > 0;
        }

        template <typename P, size_t Align = sizeof(P)>
        const P* cast()
        {
            if (_size % Align != 0)
            {
                throw Exception(
                    "Data for cast is misaligned by ",
                    (_size % Align),
                    " bytes");
            }
            return (const P*)_data;
        }

    protected:
        ArrayBase() = default;

        ArrayBase(const ArrayBase& o)
        {
            replicate(o);
        }

        explicit ArrayBase(const SizeType& initialCapacity)
        {
            reserve(initialCapacity);
        }

        ~ArrayBase() = default;

        void replicate(const ArrayBase& rhs)
        {
            if (this != &rhs)
            {
                destroy();

                if (rhs._capacity > 0 && rhs._capacity < _alloc.limit)
                {
                    reserve(rhs._capacity);
                    ConstPointerType pt = rhs.data();
                    for (_size = 0; _size < rhs._size && _data; ++_size)
                        _data[_size] = pt[_size];
                }
                else
                {
                    _data = 0;
                    _size = _capacity = 0;
                }
            }
        }

        void destroy()
        {
            if (_data)
            {
                _alloc.deallocateArray(_data, _capacity);
                _data = nullptr;
            }

            _capacity = _size = 0;
        }
    };

}  // namespace Jam
