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
#include <type_traits>

namespace Jam
{
    template <typename Ptr>
    class ScopePtr
    {
    private:
        Ptr _data{nullptr};

        ScopePtr(const ScopePtr<Ptr>&)
        {
            // not copyable
        }

    public:
        ScopePtr() = default;

        explicit ScopePtr(Ptr ptr) :
            _data{std::move(ptr)}
        {
        }

        ~ScopePtr()
        {
            delete _data;
            _data = nullptr;
        }

        Ptr ref()
        {
            return _data;
        }

        Ptr ref() const
        {
            return _data;
        }

        Ptr operator->()
        {
            return _data;
        }

        bool operator!=(Ptr rhs)
        {
            return _data != rhs;
        }

        bool isValid()
        {
            return _data != nullptr;
        }

        bool isNull()
        {
            return _data == nullptr;
        }
    };
}  // namespace Jam
