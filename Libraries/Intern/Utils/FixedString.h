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
#include "Utils/Array.h"
#include "Utils/Char.h"
#include "Utils/Hash.h"

namespace Jam
{
    template <const uint16_t L>
    class FixedString
    {
    public:
        typedef char Pointer[L + 1];

    private:
        Pointer        _buffer;
        uint16_t       _size;
        mutable hash_t _hash;

        void copy(const char*     src,
                  const uint16_t& size,
                  const uint16_t& other = JtNpos16)
        {
            const uint16_t val = Min(size, Min(L, other));

            _size = 0;
            _hash = JtNpos;

            while (_size < val && src[_size])
            {
                _buffer[_size] = src[_size];
                ++_size;
            }

            _buffer[_size] = 0;
        }

    public:
        FixedString() :
            _size(0),
            _hash(-1)
        {
            _buffer[_size] = 0;
        }

        FixedString(const FixedString& rhs) :
            _size(0),
            _hash(JtNpos)
        {
            if (rhs.size())
                copy(rhs.c_str(), rhs.size(), rhs.size());
            else
                _buffer[_size] = 0;
        }

        explicit FixedString(const char* rhs) :
            _size(0),
            _hash(JtNpos)
        {
            if (rhs)
                copy(rhs, JtNpos16, JtNpos16);
            else
                _buffer[_size] = 0;
        }

        FixedString(const char* rhs, const uint16_t& size) :
            _size(0),
            _hash(JtNpos)
        {
            if (rhs)
                copy(rhs, size, size);
            else
                _buffer[_size] = 0;
        }

        void push_back(char ch)
        {
            if (_size >= L)
                return;

            _buffer[_size++] = ch;
            _buffer[_size]   = 0;
        }

        void resize(uint16_t ns)
        {
            if (ns <= L)
            {
                _size          = ns;
                _buffer[_size] = 0;
            }
        }

        void append(const char* str)
        {
            const uint16_t len = Clamp<uint16_t>((uint16_t)Char::length(str), 0, L);

            uint16_t       a   = 0;
            while (a < len)
                push_back(str[a++]);
        }

        void append(const FixedString& str)
        {
            const uint16_t len = str._size;

            uint16_t a = 0;
            while (a < len)
                push_back(str._buffer[a++]);
        }

        FixedString& operator=(const FixedString& rhs)
        {
            if (this != &rhs && rhs._size > 0)
                copy(rhs.c_str(), rhs.size(), rhs.size());
            return *this;
        }

        template <const uint16_t OL>
        FixedString<L>& operator=(const FixedString<OL>& o)
        {
            if (o.size() > 0)
                copy(o.c_str(), o.size(), OL);
            return *this;
        }

        FixedString operator+(const FixedString& rhs)
        {
            FixedString lhs = *this;
            lhs.append(rhs);
            return lhs;
        }

        FixedString operator+=(const FixedString& rhs)
        {
            append(rhs);
            return *this;
        }

        FixedString operator+(const char* str)
        {
            FixedString lhs = *this;
            lhs.append(str);
            return lhs;
        }

        FixedString operator+=(const char* str)
        {
            append(str);
            return *this;
        }

        const char* c_str()const
        {
            return _buffer;
        }

        char* ptr()
        {
            return _buffer;
        }

        const char* ptr()const
        {
            return _buffer;
        }

        void clear()
        {
            _buffer[0] = 0;
            _size      = 0;
        }

        bool empty()const
        {
            return _size == 0;
        }

        uint16_t size()const
        {
            return _size;
        }

        static uint16_t capacity()
        {
            return L;
        }

        char operator[](uint16_t i) const
        {
            JAM_ASSERT(i < _size && i < L);
            return _buffer[i];
        }

        char at(uint16_t i) const
        {
            JAM_ASSERT(i < _size && i < L);
            return _buffer[i];
        }

        char& operator[](uint16_t i)
        {
            JAM_ASSERT(i < _size && i < L);
            return _buffer[i];
        }

        char* at(uint16_t i)
        {
            JAM_ASSERT(i < _size && i < L);
            return _buffer[i];
        }

        hash_t hash()const
        {
            if (_hash != -1)
                return _hash;

            if (_size == 0 || !_buffer)
                return -1;

            _hash = skHash(_buffer);
            return _hash;
        }

        bool operator==(const FixedString& str) const
        {
            return hash() == str.hash();
        }

        bool operator!=(const FixedString& str) const
        {
            return hash() != str.hash();
        }
    };

}  // namespace Jam
