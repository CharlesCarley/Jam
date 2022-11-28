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

#include <cmath>
#include "Utils/Array.h"

namespace Jam
{
    using R64      = double;
    using R32      = float;
    using R32Array = SimpleArray<R32>;

    constexpr R64 E64  = 2.7182818284590452353602874713527;
    constexpr R64 Pi64 = 3.1415926535897932384626433832795;

    constexpr R32 Pi  = R32(Pi64);
    constexpr R32 Pi2 = R32(2.0) * Pi;

    constexpr R32 UnitMax = 1E+09F;
    constexpr R32 UnitMin = 1E-09F;

    constexpr R32 Infinity = FLT_MAX;
    constexpr R32 Epsilon  = FLT_EPSILON;
    constexpr R32 Half     = 0.5f;
    constexpr R32 Forth    = 0.25f;
    constexpr R32 Eighth   = 0.125f;

    constexpr R64 Infinity64 = DBL_MAX;
    constexpr R64 Epsilon64  = DBL_EPSILON;

    inline void sinCos(const R32& theta, R32& y, R32& x)
    {
        x = cos(theta);
        y = sin(theta);
    }

    inline R32 minF(const R32& a, const R32& b)
    {
        return a > b ? b : a;
    }

    inline R32 maxF(const R32& a, const R32& b)
    {
        return a > b ? a : b;
    }

    inline R32 clampF(const R32& v, const R32& mi, const R32& ma)
    {
        return maxF(mi, minF(ma, v));
    }

    inline R32 div(const R32& n,
                   const R32& d,
                   const R32  def = Epsilon)
    {
        if (fabs(d) < Epsilon)
            return def;
        return n / d;
    }

    inline R32 interpolate(const R32& a,
                           const R32& b,
                           R32        s)
    {
        s = maxF(minF(s, 1.f), 0.f);
        return (1 - s) * a + b * s;
    }

    inline R32 reciprocal(const R32& value,
                          const R32  def = Epsilon,
                          const R32  tol = Epsilon)
    {
        if (fabs(value) < tol)
            return def;
        return R32(1) / value;
    }

    inline R64 reciprocal(const R64& value,
                          const R64  def = Epsilon64,
                          const R64  tol = Epsilon64)
    {
        if (fabs(value) < tol)
            return def;
        return 1.0 / value;
    }

    inline R32 sign(const R32& value)
    {
        return value < R32(0) ? R32(-1) : R32(1);
    }

    inline bool equals(const R32& a,
                       const R32& b,
                       const R32  tolerance = Epsilon)
    {
        if (std::isnan(a) && std::isnan(b))
            return true;
        return fabs(a - b) < tolerance;
    }

    inline bool equals(const R64& a,
                       const R64& b,
                       const R64  tolerance = Epsilon64)
    {
        if (std::isnan(a) && std::isnan(b))
            return true;
        return fabs(a - b) < tolerance;
    }

    inline bool inRange(const R32& a,
                        const R32  tolerance = Epsilon)
    {
        return a >= tolerance && a <= 1.f + tolerance;
    }

    inline bool isZero(const R32& a,
                       const R32  tolerance = Epsilon)
    {
        return fabs(a) < tolerance;
    }

    inline bool notZero(const R32& a,
                        const R32  tolerance = Epsilon)
    {
        return fabs(a) > tolerance;
    }

}  // namespace Jam
