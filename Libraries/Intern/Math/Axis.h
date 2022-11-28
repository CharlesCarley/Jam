#pragma once
#include "Math/Slice.h"
#include "Math/Vec2F.h"

namespace Jam
{
    class Axis
    {
    public:
        Slice x{1, 1};
        Slice y{1, 1};

        Axis() = default;

        Axis(const Axis& rhs)
        {
            set(rhs);
        }

        void set(const Axis& ax);

        void set(U8 idx, const Vec2F& ax);

        bool scale(U8 ax, U8 idx, I32 count);

        void print() const;

    };

}  // namespace Jam
