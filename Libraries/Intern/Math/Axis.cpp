#include "Math/Axis.h"
#include "Lg.h"

namespace Jam
{
    void Axis::set(const Axis& ax)
    {
        x = ax.x;
        y = ax.y;
    }

    void Axis::set(const U8 idx, const I32& ax)
    {
        switch (idx)
        {
        case 0:
            x = {};
            x.setStep(ax);
            break;
        case 1:
            y = {};
            y.setStep(ax);
            break;
        default:
            break;
        }
    }

}  // namespace Jam
