#pragma once
#include "Box.h"
#include "Math/Vec2F.h"
#include "Math/Vec2I.h"

namespace Jam
{
    inline Vec2I toVec2I(const Vec2F& v)
    {
        return {I32(v.x), I32(v.y)};
    }

    inline Vec2U toVec2U(const Vec2F& v)
    {
        return {
            U32(v.x),  // TODO: should be maxed first
            U32(v.y),
        };
    }

    inline Vec2F toVec2F(const Vec2I& v)
    {
        return {R32(v.x), R32(v.y)};
    }

    inline Vec2F toVec2F(const Vec2U& v)
    {
        return {R32(v.x), R32(v.y)};
    }

    inline RectF toRect(const Box& box)
    {
        return {box.x1, box.y1, box.x2 - box.x1, box.y2 - box.y1};
    }

    inline Box toBox(const RectF& rect)
    {
        return {rect.x, rect.y, rect.x + rect.w, rect.y + rect.h};
    }

}  // namespace Jam
