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
#include <utility>
#include "Utils/String.h"

namespace Jam::Xml
{
    /**
     * \brief Provides a key-value representation of an XML attribute.
     */
    class Attribute
    {
    private:
        String _key;
        String _value;

    public:
        /**
         * \brief Default constructor.
         * \param key The lookup key for this attribute.
         * \param value The value to associate with the supplied key.
         */
        Attribute(String key, String value);

        /**
         * \brief Provides const access to the key field.
         * \return The key associated with this attribute.
         */
        [[nodiscard]] const String& key() const;

        /**
         * \brief Provides const access to the value field.
         * \return The value associated with this attribute.
         */
        [[nodiscard]] const String& value() const;
    };

    inline Attribute::Attribute(String key, String value) :
        _key(std::move(key)),
        _value(std::move(value))
    {
    }

    inline const String& Attribute::key() const
    {
        return _key;
    }

    inline const String& Attribute::value() const
    {
        return _value;
    }

}  // namespace Jam::Xml
