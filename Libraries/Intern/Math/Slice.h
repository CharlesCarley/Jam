#pragma once

#include "Math/Vec2F.h"

namespace Jam
{

    template <U32 N>  // [N..1],[1..1/N]
    class SliceT
    {
    public:
        const R64 low = R64(1) / R64(N);

    private:
        U32 _n{1};
        U32 _d{1};
        I32 _c{1};
        I32 _c10{10};

        static I32 m8(const I32 x)
        {
            return 8 - x % 8;
        }

        static I32 m2(const I32 x)
        {
            return x % 2;
        }

        void clampC10()
        {
            // https://www.desmos.com/calculator/futo1mo0pf
            constexpr R32 s = 1.f / 0.5f;
            const I32     x = I32(floor(R32(Abs(_c)) * s));

            _c10 = 2 + (m8(x) + m2(x));
        }

        void clamp()
        {
            _c = Clamp(_c, -I32(N), I32(N));

            if (_c < 0)
            {
                _d = Abs(_c);
                _n = 1;
            }
            else if (_c > 0)
            {
                _d = 1;
                _n = _c;
            }
            else
            {
                _d = 1;
                _n = 1;
                _c = 1;
            }
        }

    public:
        SliceT() = default;
        SliceT(const SliceT<N>& rhs)
        {
            _n = rhs._n;
            _d = rhs._d;
            _c = rhs._c;
            clamp();
            clampC10();
        }

        SliceT& operator=(const SliceT<N>& rhs)
        {
            _n = rhs._n;
            _d = rhs._d;
            _c = rhs._c;
            clamp();
            clampC10();
            return *this;
        }

        void setStep(const I32 i)
        {
            _c = i;
            clamp();
            clampC10();
        }

        void inc()
        {
            if (++_c == 0) ++_c;
            clamp();
            clampC10();
        }

        void dec()
        {
            if (--_c == 0) --_c;
            clamp();
            clampC10();
        }

        R64 value() const
        {
            return R64(_n) + (low - R64(_d) / R64(N));
        }

        I32 mod(const I32 n) const
        {
            if (n != 0)
                return _c % n;
            return 0;
        }

        R32 rn() const { return R32(_c) + R32(N); }
        R32 r10() const { return R32(_c10); }
        R32 rc() const { return R32(_c); }

        R32 pointBy(const R32 x) const
        {
            return R32(_n) * x / R32(_d);
        }

        R32 pointByI(const R32 x) const
        {
            return R32(_d) * x / R32(_n);
        }
    };

    using Slice = SliceT<1'000'000>;

}  // namespace Jam
