#pragma once
#include <cstdint>
#include "Utils/Definitions.h"

namespace Jam
{
    class TokenBase
    {
    private:
        size_t _index{JtNpos};
        int8_t _type{0};
        size_t _line{0};

    public:
        TokenBase() = default;

        TokenBase(const TokenBase& tok) = default;

        ~TokenBase() = default;

        void clear()
        {
            // Reset the lookup index
            // Its range is any value >= 0 && < structure.size()
            _index = JtNpos;

            // the type here should pivot around zero,
            // negative values are errors, positive values are
            // possible tokens, and 0 indicates null
            _type = 0;
            _line = 0;
        }

        [[nodiscard]] size_t index() const;

        [[nodiscard]] int8_t type() const;

        [[nodiscard]] size_t line() const;

        void setType(int8_t type);

        void setIndex(size_t i);

        void setLine(size_t line);
    };

    inline size_t TokenBase::index() const
    {
        return _index;
    }

    inline int8_t TokenBase::type() const
    {
        return _type;
    }

    inline size_t TokenBase::line() const
    {
        return _line;
    }

    inline void TokenBase::setType(const int8_t type)
    {
        _type = type;
    }

    inline void TokenBase::setIndex(const size_t i)
    {
        _index = i;
    }

    inline void TokenBase::setLine(const size_t line)
    {
        _line = line;
    }

}  // namespace Jam
