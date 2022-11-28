#pragma once

#include "HashMap.h"
#include "Allocator.h"
#include "Hash.h"
#include "Traits.h"
#include "Definitions.h"

namespace Jam
{
    template <typename T, typename Alloc = Allocator<Entry<T, bool>, size_t> >
    class Set
    {
    public:
        using TableType = HashTable<T, bool, Alloc>;

        JAM_DECLARE_REF_TYPE(TableType)

        using SelfType = Set<T, Alloc>;

    public:
        Set()  = default;
        ~Set() = default;

        Set(const Set& oth) :
            _table(oth)
        {
        }

        void clear()
        {
            _table.clear();
        }

        bool insert(const T& v)
        {
            return _table.insert(v, true);
        }

        void erase(const T& v)
        {
            _table.remove(v);
        }

        size_t find(const T& v)
        {
            return _table.find(v);
        }

        T& operator[](size_t idx)
        {
            JAM_ASSERT(idx < size());
            return _table.at(idx);
        }

        const T& operator[](size_t idx) const
        {
            JAM_ASSERT(idx < size());
            return _table.at(idx);
        }

        T& at(size_t idx)
        {
            JAM_ASSERT(idx < size());
            return _table.at(idx);
        }

        const T& at(size_t idx) const
        {
            JAM_ASSERT(idx < size());
            return _table.at(idx);
        }

        size_t size() const
        {
            return _table.size();
        }

        bool empty() const
        {
            return _table.empty();
        }

        PointerType data()
        {
            return _table.data();
        }

        ConstPointerType data() const
        {
            return _table.data();
        }

        Set& operator=(const Set& rhs);

    private:
        TableType _table;
    };

    template <typename T, typename Allocator>
    Set<T, Allocator>& Set<T, Allocator>::operator=(const Set& rhs)
    {
        if (this != &rhs)
            _table = rhs._table;
        return *this;
    }
}  // namespace Jam
