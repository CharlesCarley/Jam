#include "Math/Axis.h"
#include "Lg.h"

namespace Jam
{
    void Axis::set(const Axis& ax)
    {
        x = ax.x;
        y = ax.y;
    }

    void Axis::set(const U8 idx, const Vec2F& ax)
    {
        switch (idx)
        {
        case 0:
            x = {(int)ax.x, (int)ax.y};
            break;
        case 1:
            y = {(int)ax.x, (int)ax.y};
            break;
        default:
            break;
        }
    }

    bool Axis::scale(const U8 ax, const U8 idx, const I32 count)
    {
        if (ax == 0)
        {
            if (idx == 0)
                x.n(count);
            else
                x.d(count);
        }
        else
        {
            if (idx == 0)
                y.n(count);
            else
                y.d(count);
        }
        return true;
    }

    void Axis::print() const
    {
        x.print();
        y.print();
    }
}  // namespace Jam
