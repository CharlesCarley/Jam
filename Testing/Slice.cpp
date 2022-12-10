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
#include "Math/Slice.h"
#include "gtest/gtest.h"

using namespace Jam;

GTEST_TEST(Slice, Slice01)
{
    SliceT<1000> s;
    int          i = 0;
    s.setStep(1000);

    EXPECT_DOUBLE_EQ(s.value(), 1000);

    while (i < 2000)
    {
        EXPECT_LE(s.r10(), 10);
        EXPECT_GE(s.r10(), 4);
        EXPECT_EQ(remainder(s.r10(), 2), 0);

        //Dbg::println(s.r10());
        s.dec();
        ++i;
    }
    EXPECT_DOUBLE_EQ(s.value(), 0.001);
}
