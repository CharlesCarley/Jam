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
#include "Math/Domain.h"
#include "Math/Lg.h"
#include "Math/Real.h"

namespace Jam
{
    template <typename I1N>
    class Vec2T
    {
    public:
        using D = TDomain<I1N>;

    public:
        I1N x{0};
        I1N y{0};

        Vec2T()               = default;
        Vec2T(const Vec2T& v) = default;
        constexpr Vec2T(const I1N& a,
                        const I1N& b) :
            x(a), y(b)
        {
        }

        I1N product() const
        {
            return Min<I1N>(I64(x * y), D::Ub);
        }

        I1N xMod(I1N ix) const
        {
            return Mod(ix, x);
        }

        I1N yMod(I1N iy) const
        {
            return Mod(iy, y);
        }

        I1N overX(I1N ix) const
        {
            return x > 0 ? ix / x : 0;
        }

        I1N overY(I1N iy) const
        {
            return y > 0 ? iy / y : 0;
        }

        R32 m() const
        {
            return x > 0 ? R32(y) / R32(x) : 0;
        }

        R32 rx() const
        {
            return R32(x);
        }

        R32 ry() const
        {
            return R32(y);
        }

        I1N combine(I1N nx, I1N ny) const
        {
            return Clamp<I1N>(
                ny % y * x + nx,
                0,
                x * y);
        }

        I64 combineI(const I64 nx, const I64 ny) const
        {
            const I64 u = x * I64(y);
            return Clamp<I64>(
                ny * I64(x) + nx, -u, u);
        }

        Vec2T<I1N> half() const
        {
            return {x >> 1, y >> 1};
        }

        I1N length() const
        {
            R32 d = R32(x) * R32(x) + R32(y) * R32(y);
            return (I1N)d;
        }

        bool operator!=(const Vec2T& rhs) const
        {
            return x != rhs.x || y != rhs.y;
        }

        bool operator==(const Vec2T& rhs) const
        {
            return x == rhs.x && y == rhs.y;
        }

        void print() const
        {
            Lg::print2I(x, y);
        }

        I1N sum() const
        {
            return x + y;
        }

        Vec2T<I32> operator*(I32 i) const
        {
            return {x * i, y * i};
        }

        R32 dmm()
        {
            return R32(Max(y, x) - Min(y, x));
        }
    };

    using Vec2I = Vec2T<I32>;
    using Vec2U = Vec2T<U32>;
}  // namespace Jam
