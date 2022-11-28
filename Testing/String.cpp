#include "Utils/String.h"
#include <gtest/gtest.h>
#include "Math/Lg.h"

using namespace Jam;

GTEST_TEST(String, Trim)
{
    const String a = "H e l l o";
    String       x;

    Su::trimWs(x, a);
    EXPECT_EQ(x, a);

    const String b = " H e l l o";

    Su::trimWs(x, b);
    EXPECT_EQ(x, a);

    const String c = "H e l l o ";

    Su::trimWs(x, c);
    EXPECT_EQ(x, a);

    const String d = "     H e l l o        ";
    Su::trimWs(x, d);
    EXPECT_EQ(x, a);

    String inp = "     A B C       ";
    Su::trimWs(inp, inp);
    EXPECT_EQ("A B C", inp);

    inp = "\t\t\t\tA\tB\tC\t\t\t\t\t";
    Su::trimWs(inp, inp);
    EXPECT_EQ("A\tB\tC", inp);

    inp = " \t \t \tA\tB C \t \t  \t";
    Su::trimWs(inp, inp);
    EXPECT_EQ("A\tB C", inp);

    inp = "";
    Su::trimWs(inp, inp);
    EXPECT_EQ("", inp);

    inp = " 111 111 1111  3 ";
    Su::trimWs(inp, inp);
    EXPECT_EQ("111 111 1111  3", inp);

}
