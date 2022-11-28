#pragma once

#include "Utils/Array.h"

namespace Jam
{
    template <typename T, const uint16_t L>
    class FixedArray
    {
    public:
        JAM_DECLARE_TYPE(T)
        static const uint16_t limit = L;

    protected:
        mutable ValueType _data[L + 1];

        uint16_t _size;

        template <const uint16_t UpperL, const uint16_t UpperR>
        static void copy(FixedArray<T, UpperL>& lhs, const FixedArray<T, UpperR>& rhs)
        {
            if (rhs.empty())
                return;

            lhs.clear();

            const uint16_t upperBound = Min(UpperL, UpperR);

            for (uint16_t current = 0; current < upperBound; ++current)
                lhs.push_back(rhs.at(current));
        }

    public:
        FixedArray() :
            _size(L)
        {
        }

        explicit FixedArray(const T& v) :
            _size(L)
        {
            Fill(_data, v, L);
        }

        void push_back(ConstReferenceType rhs)
        {
            if (_size + 1 >= L)
                return;

            _data[_size++] = rhs;
        }

        uint16_t find(ConstReferenceType v)
        {
            for (uint16_t current = 0; current < _size; current++)
            {
                if (_data[current] == v)
                    return current;
            }

            return JtNpos16;
        }

        void remove(uint16_t pos)
        {
            if (pos < _size && pos < L)
                return;

            if (_size < 1)
                return;

            uint16_t end = _size - 1;
            Swap(_data[pos], _data[end]);
            _data[end].~T();

            --_size;
        }

        PointerType begin()
        {
            return &_data[0];
        }

        PointerType end()
        {
            JAM_ASSERT(_size < L)
            PointerType start = &_data[0];
            return start + _size;
        }

        ConstPointerType begin() const
        {
            return &_data[0];
        }

        ConstPointerType end() const
        {
            JAM_ASSERT(_size < L)
            PointerType start = &_data[0];
            return start + _size;
        }

        void resize(uint16_t size)
        {
            if (size >= L || _size >= L)
                return;

            if (size < _size)
            {
                for (uint16_t current = size; current < _size; current++)
                    _data[current].~T();
            }
            else
            {
                for (uint16_t current = 0; current < size; current++)
                    _data[current] = T();
            }

            _size = size;
        }

        void clear()
        {
            resize(0);
        }

        PointerType ptr()
        {
            return &_data[0];
        }

        ConstPointerType ptr() const
        {
            return &_data[0];
        }

        bool empty() const
        {
            return _size <= 0;
        }

        uint16_t size() const
        {
            return _size;
        }

        ReferenceType back()
        {
            JAM_ASSERT(!empty())
            return _data[_size - 1];
        }

        ConstReferenceType back() const
        {
            JAM_ASSERT(!empty())
            return _data[_size - 1];
        }

        static uint16_t capacity()
        {
            return L;
        }

        ConstReferenceType operator[](uint16_t idx) const
        {
            JAM_ASSERT(idx < _size && idx < L)
            return _data[idx];
        }

        ConstReferenceType at(uint16_t idx) const
        {
            JAM_ASSERT(idx < _size && idx < L)
            return _data[idx];
        }

        ReferenceType operator[](uint16_t idx)
        {
            JAM_ASSERT(idx < _size && idx < L)
            return _data[idx];
        }

        ReferenceType at(uint16_t idx)
        {
            JAM_ASSERT(idx < _size && idx < L)
            return _data[idx];
        }
    };

}  // namespace Jam
