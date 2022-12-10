#pragma once
#include "Math/Slice.h"
#include "Math/Vec2F.h"

namespace Jam
{
    class Axis
    {
    public:
        Slice x{};
        Slice y{};

        Axis() = default;

        Axis(const Axis& rhs)
        {
            set(rhs);
        }

        R32 xMajor() const { return x.r10(); }

        R32 yMajor() const { return y.r10(); }

        void set(const Axis& ax);

        void set(U8 idx, const I32& ax);
    };

}  // namespace Jam
