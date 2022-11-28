#pragma once
#include "Math/Integer.h"
#include "Math/Real.h"

namespace Jam
{
    class Vec2F
    {
    public:
        R32 x{}, y{};
        Vec2F() = default;

        Vec2F(const Vec2F& rhs) = default;

        constexpr Vec2F(const R32& ix,
                        const R32& iy) :
            x{ix}, y{iy}
        {
        }

        explicit constexpr Vec2F(
            const U32& ux,
            const U32& uy) :
            x{R32(ux)}, y{R32(uy)}
        {
        }

        R32 length() const
        {
            if (const R32 len = length2();
                !equals(len, 0))
                return powf(len, -Half);
            return 0;
        }

        R32 length2() const
        {
            return dot(*this);
        }

        R32 dot(const Vec2F& rhs) const
        {
            return x * rhs.x + y * rhs.y;
        }

        Vec2F reciprocal() const
        {
            return {
                Jam::reciprocal(x),
                Jam::reciprocal(y),
            };
        }

        Vec2F perpendicular() const
        {
            return {-y, x};
        }

        R32 distance2(const Vec2F& oth) const
        {
            return Vec2F(x - oth.x, y - oth.y).length2();
        }

        R32 distance(const Vec2F& oth) const
        {
            return Vec2F(x - oth.x, y - oth.y).length();
        }

        R32 slope(const Vec2F& oth) const
        {
            return div(oth.y - y, oth.x - x, 0);
        }

        Vec2F interpolate(const Vec2F& oth,
                          const R32    tol) const
        {
            return {
                Jam::interpolate(x, oth.x, tol),
                Jam::interpolate(y, oth.y, tol),
            };
        }

        void normalize()
        {
            if (R32 len = length2();
                !equals(len, 0))
            {
                len = powf(len, -Half);
                x *= len;
                y *= len;
            }
        }

        Vec2F normalized() const
        {
            Vec2F v(*this);
            v.normalize();
            return v;
        }

        void print(U8 p = 3) const;

        Vec2F operator*(const R32 v) const
        {
            return {x * v, y * v};
        }

        Vec2F operator+(const Vec2F& v) const
        {
            return {x + v.x, y + v.y};
        }

        Vec2F operator-(const Vec2F& v1) const
        {
            return {x - v1.x, y - v1.y};
        }

        Vec2F operator/(const R32 v) const
        {
            return operator*(
                Jam::reciprocal(v, 0));
        }

        bool isNotZero() const
        {
            return !(isZero(x) && isZero(y));
        }

        bool operator!=(const Vec2F& size) const;

        Vec2F& operator/=(R32 v)
        {
            v = Jam::reciprocal(v, 0);
            x *= v;
            y *= v;
            return *this;
        }

        Vec2F absolute() const
        {
            return {fabs(x), fabs(y)};
        }

        Vec2F& operator+=(const Vec2F& v)
        {
            x += v.x;
            y += v.y;
            return *this;
        }

        Vec2F& operator*=(const Vec2F& v)
        {
            x *= v.x;
            y *= v.y;
            return *this;
        }

        Vec2F operator+(R32 scale) const
        {
            const R32 sc = Jam::reciprocal(scale, 0);
            return {x * sc, y * sc};
        }

        void clamp(const R32 v0, const R32 v1)
        {
            x = clampF(x, v0, v1);
            y = clampF(y, v0, v1);
        }

        void clamp(const Vec2F& v)
        {
            clamp(v.x, v.y);
        }

        I32 ix() const { return I32(x); }
        I32 iy() const { return I32(y); }
    };

}  // namespace Jam