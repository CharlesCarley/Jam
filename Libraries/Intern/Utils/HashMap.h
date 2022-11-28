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
#include "Utils/Hash.h"

namespace Jam
{
    template <typename Key, typename Value>
    class Entry
    {
    public:
        Key    first{};
        Value  second{};
        hash_t hash{JtNpos};

        Entry()  = default;
        ~Entry() = default;

        Entry(const Key& k, const Value& v, hash_t hk) :
            first(k),
            second(v),
            hash(hk)
        {
        }

        Entry(const Entry& oth) :
            first(oth.first),
            second(oth.second),
            hash(oth.hash)
        {
        }
    };

    // Derived from btHashTable
    // https://github.com/bulletphysics/bullet3/blob/master/src/LinearMath/btHashMap.h
    template <typename Key,
              typename Value,
              typename Alloc = Allocator<Entry<Key, Value>, size_t> >
    class HashTable
    {
    public:
        typedef Allocator<size_t>            IndexAllocator;
        typedef HashTable<Key, Value, Alloc> SelfType;

    public:
        typedef Entry<Key, Value> Pair;
        JAM_DECLARE_TYPE(Pair)

        typedef size_t* IndexArray;
        typedef Key     PairKeyType;
        typedef Value   PairValueType;

    private:
        Alloc          _alloc;
        IndexAllocator _iAlloc;
        size_t         _size{0};
        size_t         _capacity{0};
        IndexArray     _indices{nullptr};
        IndexArray     _next{nullptr};
        PointerType    _bucket{nullptr};

    public:
        HashTable() = default;

        explicit HashTable(const size_t& initialCapacity)
        {
            reserve(initialCapacity);
        }

        HashTable(const HashTable& rhs)
        {
            copy(rhs);
        }

        ~HashTable()
        {
            clear();
        }

        SelfType& operator=(const SelfType& rhs)
        {
            if (this != &rhs)
                copy(rhs);
            return *this;
        }

        void clear()
        {
            if (_bucket)
            {
                _alloc.deallocateArray(_bucket, _capacity);
                _bucket = nullptr;
            }

            if (_indices)
            {
                _iAlloc.deallocateArray(_indices, _capacity);
                _indices = nullptr;
            }

            if (_next)
            {
                _iAlloc.deallocateArray(_next, _capacity);
                _next = nullptr;
            }
            _size = _capacity = 0;
        }

        Value& at(size_t i)
        {
            JAM_ASSERT(_bucket && i < _size)
            return _bucket[i].second;
        }

        Value& operator[](size_t i)
        {
            JAM_ASSERT(_bucket && i < _size)
            return _bucket[i].second;
        }

        const Value& at(size_t i) const
        {
            JAM_ASSERT(_bucket && i < _size)
            return _bucket[i].second;
        }

        const Value& operator[](size_t i) const
        {
            JAM_ASSERT(_bucket && i < _size)
            return _bucket[i].second;
        }

        Key& keyAt(size_t i)
        {
            JAM_ASSERT(_bucket && i < _size)
            return _bucket[i].first;
        }

        const Key& keyAt(size_t i) const
        {
            JAM_ASSERT(_bucket && i < _size)
            return _bucket[i].first;
        }

        Value& get(const Key& key)
        {
            size_t i = find(key);
            if (i == JtNpos)
                throw NotFound();
            return _bucket[i].second;
        }

        Value& operator[](const Key& key)
        {
            return get(key);
        }

        const Value& operator[](const Key& key) const
        {
            return get(key);
        }

        size_t find(const Key& key) const
        {
            if (empty())
                return JtNpos;

            hash_t       hk = Hash(key);
            const hash_t hr = hk & _capacity - 1;
            size_t       fh = _indices[hr];

            while (fh != JtNpos && hk != _bucket[fh].hash)
                fh = _next[fh];

            return fh;
        }

        bool insert(const Key& key, const Value& val)
        {
            if (!empty())
            {
                if (find(key) != JtNpos)
                    return false;
            }

            if (_size == _capacity)
                reserve(_size == 0 ? 32 : _size * 2);

            hash_t       hk = Hash(key);
            const hash_t hr = hk & _capacity - 1;

            _bucket[_size] = Pair(key, val, hk);
            _next[_size]   = _indices[hr];
            _indices[hr]   = _size++;
            return true;
        }

        void erase(const Key& key)
        {
            remove(key);
        }

        void remove(const Key& key)
        {
            if (empty())
                return;

            size_t       fIndex = find(key);
            const hash_t hash   = Hash(key) & _capacity - 1;

            size_t index  = _indices[hash];
            size_t pIndex = JtNpos;

            while (index != fIndex)
            {
                pIndex = index;
                index  = _next[index];
            }

            if (pIndex != JtNpos)
            {
                JAM_ASSERT(_next[pIndex] == fIndex)
                _next[pIndex] = _next[fIndex];
            }
            else
                _indices[hash] = _next[fIndex];

            size_t lIndex = _size - 1;
            if (lIndex == fIndex)
            {
                --_size;
                _bucket[_size].~Pair();
                return;
            }

            const hash_t lHash = _bucket[lIndex].hash & _capacity - 1;
            index              = _indices[lHash];

            pIndex = JtNpos;
            while (index != lIndex)
            {
                pIndex = index;
                index  = _next[index];
            }

            if (pIndex != JtNpos)
            {
                JAM_ASSERT(_next[pIndex] == lIndex)
                _next[pIndex] = _next[lIndex];
            }
            else
                _indices[lHash] = _next[lIndex];

            _bucket[fIndex] = _bucket[lIndex];
            _next[fIndex]   = _indices[lHash];
            _indices[lHash] = fIndex;

            --_size;
            _bucket[_size].~Pair();
        }

        PointerType data()
        {
            return _bucket;
        }

        ConstPointerType data() const
        {
            return _bucket;
        }

        bool valid() const
        {
            return _bucket != nullptr;
        }

        size_t size() const
        {
            return _size;
        }

        size_t capacity() const
        {
            return _capacity;
        }

        bool empty() const
        {
            return _size <= 0;
        }

        void reserve(const size_t& nr)
        {
            if (_capacity < nr && nr != JtNpos)
                rehash(nr);
        }

        Entry<Key, Value>* begin() const
        {
            return _bucket;
        }

        Entry<Key, Value>* end() const
        {
            return _bucket + _size;
        }

    private:
        void zeroIndices(const size_t& from, const size_t& to) const
        {
            if (to <= 0 || from >= to)
                return;
#ifdef JAM_OPEN_MP
            {
#pragma omp parallel for num_threads(4)
                for (int64_t i = from; i < (int64_t)to; ++i)
                {
                    _indices[i] = _next[i] = JtNpos;
                }
            }
#else
            size_t i = from;
            do
            {
                _indices[i] = _next[i] = JtNpos;
            } while (++i < to);
#endif
        }

        void copy(const SelfType& rhs)
        {
            if (rhs.valid() && !rhs.empty())
            {
                reserve(rhs._capacity);

                _size     = rhs._size;
                _capacity = rhs._capacity;

                const size_t b = _size > 0 ? _size - 1 : 0;
                zeroIndices(b, _capacity);

                for (size_t i = 0; i < _size; ++i)
                {
                    _bucket[i]  = rhs._bucket[i];
                    _indices[i] = rhs._indices[i];
                    _next[i]    = rhs._next[i];
                }
            }
        }

        void rehash(size_t nr)
        {
            if (!IsPow2(nr))
                NextPow2(nr);

            _bucket  = _alloc.reallocateArray(_bucket, nr, _capacity);
            _indices = _iAlloc.reallocateArray(_indices, nr, _capacity);
            _next    = _iAlloc.reallocateArray(_next, nr, _capacity);

            _capacity = nr;
            JAM_ASSERT(_bucket && _indices && _next)

            zeroIndices(0, _capacity);

            for (hash_t i = 0; i < _size; i++)
            {
                const hash_t h = _bucket[i].hash & _capacity - 1;
                _next[i]       = _indices[h];
                _indices[h]    = i;
            }
        }
    };

}  // namespace Jam
