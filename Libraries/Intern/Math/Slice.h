#pragma once

#include "Math/Vec2F.h"

namespace Jam
{
    constexpr I32 SliceHigh = 0xFFFFFE;
    constexpr R32 SliceLow  = 1.f / R32(SliceHigh);

    class Slice  // [0...1]
    {
    private:
        U32 _n{1};
        U32 _d{1};
        I32 _c{0};
        I8  _s{0};

    public:
        Slice() = default;
        Slice(const Slice& rhs)
        {
            set(I32(rhs._n), I32(rhs._d));
        }

        Slice(const I32& n, const I32& d)
        {
            set(n, d);
        }

        void set(const I32& n, const I32& d);

        bool validate() const
        {
            return _n >= 1 && _d >= 1;
        }

        void step(I32 delta);
        void stepI(I32 value);

        R32 upper() const
        {
            return R32(_n) * reciprocal(R32(_d));
        }

        R32 lower() const
        {
            return R32(_d) * reciprocal(R32(_n));
        }

        R32 fraction() const
        {
            const R32 f = (SliceLow - R32(_d) / R32(SliceHigh));
            return R32(sign()) * R32(_n) + f;
        }

        R32 inverse() const
        {
            return reciprocal(fraction());
        }

        R32 max() const
        {
            return R32(Max(_n, _d));
        }

        R32 mod(I32 x) const
        {
            if (_d < _n)
                return R32(_n % x);
            return R32(_d % x);
        }

        R32 pointBy(const R32 x) const
        {
            return R32(_n) * x / R32(_d);
        }

        R32 pointByI(const R32 x) const
        {
            return R32(_d) * x / R32(_n);
        }

        void n(const I32& n)
        {
            set(n, I32(_d));
        }

        void d(const I32& d)
        {
            set(I32(_n), d);
        }

        I8 sign() const { return _s; }

        const U32& n() const { return _n; }
        const U32& d() const { return _d; }

        static I8 sign(const I32& n, const I32& d)
        {
            return n * d >= 0 ? 1 : -1;
        }

        void print(U8 p = 3) const;

        
        Vec2F toVec2F() const
        {
            return Vec2F{_n, _d};
        }
    };

}  // namespace Jam
