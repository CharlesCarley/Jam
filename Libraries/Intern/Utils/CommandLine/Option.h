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

#include <cassert>
#include "Utils/Char.h"
#include "Utils/String.h"

namespace Jam::CommandLine
{
    /**
     * \brief A structure that represents a command line switch.
     */
    struct Switch
    {
        /**
         * \brief Defines the lookup id for this switch. 
         */
        uint32_t id;

        /**
         * \brief Assigns a single character value for this switch -[a-zA-Z].
         * 
         * A value of 0 means unused.
         */
        char shortSwitch;

        /**
         * \brief Assigns word or phrase value for this switch --[a-zA-Z]+
         * 
         * A value of null means unused.
         */
        const char* longSwitch;

        /**
         * \brief Provides A brief description for the -h, --help builtin option.
         */
        const char* description;

        /**
         * \brief Defines this switch as optional.
         */
        bool optional;

        /**
         * \brief Defines the number of required arguments.
         */
        size_t argCount;
    };

    class ParseOption
    {
    public:
        typedef std::vector<String> Arguments;

    private:
        const Switch _switch{};
        Arguments    _values;
        bool         _isPresent;

    public:
        ParseOption();

        explicit ParseOption(const Switch& option);

        bool isOptional() const;

        bool isPresent() const;

        size_t getArgCount() const;

        void makePresent();

        const Switch& getSwitch() const;

        const Arguments& getValues() const;

        const String& getValue(const size_t& idx = 0);

        void setValue(const String& str);

        int getInt(const size_t& idx = 0, const int& defaultValue = -1, const int& base = 10);

        int64_t getInt64(const size_t& idx = 0, const int64_t& defaultValue = -1, const int& base = 10);

        void setValue(const size_t& index, const String& str);
    };

    inline  ParseOption::ParseOption() :
        _isPresent(false)
    {
    }

    inline  ParseOption::ParseOption(const Switch& option) :
        _switch(option),
        _isPresent(false)
    {
        if (option.argCount > 0)
            _values.resize(option.argCount);
    }

    inline  bool ParseOption::isOptional() const
    {
        return _switch.optional;
    }

    inline  bool ParseOption::isPresent() const
    {
        return _isPresent;
    }

    inline  size_t ParseOption::getArgCount() const
    {
        return _values.size();
    }

    inline  void ParseOption::makePresent()
    {
        _isPresent = true;
    }

    inline  const Switch& ParseOption::getSwitch() const
    {
        return _switch;
    }

    inline  const ParseOption::Arguments& ParseOption::getValues() const
    {
        return _values;
    }

    inline  const String& ParseOption::getValue(const size_t& idx)
    {
        assert(idx < _values.size());
        return _values[idx];
    }

    inline  void ParseOption::setValue(const String& str)
    {
        if (!_values.empty())
            _values[0] = str;
    }

    inline  int ParseOption::getInt(const size_t& idx,
                                   const int&    defaultValue,
                                   const int&    base)
    {
        return Char::toInt32(getValue(idx),
                             defaultValue,
                             base);
    }

    inline  int64_t ParseOption::getInt64(const size_t&  idx,
                                          const int64_t& defaultValue,
                                          const int&     base)
    {
        return Char::toInt64(getValue(idx),
                             defaultValue,
                             base);
    }

    inline  void ParseOption::setValue(const size_t& index, const String& str)
    {
        if (index < _values.size())
            _values[index] = str;
    }
}  // namespace Jam::CommandLine
