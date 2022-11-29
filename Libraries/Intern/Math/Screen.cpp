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
#include "Math/Screen.h"

namespace Jam
{

    void Screen::reset()
    {
        _o = _i;
        _c = _v.halfExtent();
    }

    void Screen::setViewport(const RectF& v)
    {
        setViewport(v.x, v.y, v.w, v.h);
    }
    void Screen::setViewport(const I32& x, const I32& y, const I32& w, const I32& h)
    {
        setViewport(R32(x), R32(y), R32(w), R32(h));
    }

    void Screen::setViewport(const R32& x, const R32& y, const R32& w, const R32& h)
    {
        _v.x = Max(x, -ScreenMax);
        _v.y = Max(y, -ScreenMax);
        _v.w = Min(w, ScreenMax);
        _v.h = Min(h, ScreenMax);
    }

}  // namespace Jam
