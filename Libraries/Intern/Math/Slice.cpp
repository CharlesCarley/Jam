#include "Math/Slice.h"
#include "Math/Lg.h"
#include "Utils/StreamMethods.h"

namespace Jam
{
    void Slice::print(const U8 p) const
    {
        Lg::print(
            SetS({"n", "d"}),
            Equ(),
            SetU({_n, _d}, 0, 0),
            Nl(),
            SetS({"U", "L"}),
            Equ(),
            SetF({upper(), lower()}, 0, p),
            NlTab(),
            SetF({fraction()}, 32, 32, true));
    }

    void Slice::set(const I32& n, const I32& d)
    {
        const U32 a = Abs(n);
        const U32 b = Abs(d);

        _c = 0;
        _s = sign(n == 0 ? 1 : n, d);
        _n = Clamp(a, U32(1), U32(SliceHigh));
        _d = Clamp(b, U32(1), U32(SliceHigh) + 1);
    }

    void Slice::step(const I32 delta)
    {
        _c += delta;
        if (_c == 0) _c = 1;
        if (_c < 0)
        {
            _s = -1;
            _d = -_c;
            _n = 1;
        }
        else
        {
            _s = 1;
            _d = 1;
            _n = _c;
        }
    }

    void Slice::stepI(const I32 value)
    {
        if (_c = value == 0 ? 1 : value;
            _c < 0)
        {
            _s = -1;
            _d = -_c;
            _n = 1;
        }
        else
        {
            _s = 1;
            _d = 1;
            _n = _c;
        }
    }
}  // namespace Jam
