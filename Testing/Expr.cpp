#include <cstdio>
#include "Equation/Stmt.h"
#include "Equation/StmtParser.h"
#include "Equation/StmtScanner.h"
#include "ExprData.inl"
#include "Math/Lg.h"
#include "TestDirectory.h"
#include "Utils/Char.h"
#include "Utils/StreamMethods.h"
#include "gtest/gtest.h"

using namespace Jam;

///////////////////////////////////////////////////////////////////////////////
constexpr R64 DbgPi = R64(3.1415926535897932);
constexpr R64 Exit  = R64(3.018392941684311);

GTEST_TEST(Expression, Parse00c)
{
    StringStream ss;
    ss << "1000*x";

    Eq::StmtParser code;
    code.read(ss);
    logSymbols(code.symbols());

    Eq::Stmt eval;
    eval.set("x", 2);
    eval.execute(code.symbols());
    EXPECT_DOUBLE_EQ((int)eval.peek(0), 2000.0);
}

GTEST_TEST(Expression, Parse00b)
{
    StringStream ss;
    ss << "mod(cos(x), 3)";

    Eq::StmtParser code;
    code.read(ss);
    logSymbols(code.symbols());

    Eq::Stmt eval;
    eval.set("x", R64(Pi));
    eval.execute(code.symbols());
    EXPECT_DOUBLE_EQ((int)eval.peek(0), 2.0);
}

GTEST_TEST(Expression, Parse00a)
{
    StringStream ss;
    ss << "mod(x,b)";

    Eq::StmtParser code;
    code.read(ss);
    logSymbols(code.symbols());

    Eq::Stmt eval;
    eval.set("b", 5);

    int i = 0;
    do
    {
        eval.set("x", i++);
    } while (eval.execute(code.symbols()) <= 3.0 && i < 10);

    EXPECT_EQ(i, 5);
    EXPECT_DOUBLE_EQ(eval.peek(0), 4.0);
}

GTEST_TEST(Expression, Parse009)
{
    StringStream ss;

    ss << "(3.1415926535897932*x-a)/(x+b)";
    Eq::StmtParser parse;
    parse.read(ss);

    std::cout << DoublePrint(DbgPi, 0, 32);

    constexpr R64 a = 1;
    constexpr R64 b = 1;

    Eq::Stmt eval;
    int      i = 0;

    eval.set("x", i);
    eval.set("a", a);
    eval.set("b", b);

    eval.execute(parse.symbols());
    while (eval.peek(0) < Exit)
    {
        const R64 x = R64(i);
        eval.set("x", x);

        EXPECT_DOUBLE_EQ(
            //
            eval.execute(parse.symbols()),
            //
            (3.1415926535897932 * x - a) / (x + b));

        ++i;
    }

    EXPECT_EQ(i, 34);
}

GTEST_TEST(Expression, Parse008)
{
    StringStream ss;
    ss << "1.0 + 9.0 / 10.0 + 9.0 / 100.0 + 6.0 / 1000.0 + 5.0 / 10000.0";
    Eq::StmtParser parse;
    parse.read(ss);
    logSymbols(parse.symbols());
    Eq::Stmt eval;
    R64      result = eval.execute(parse.symbols());
    Dbg::println(SetD({1.9965, result}, 0, 4, true));
    EXPECT_DOUBLE_EQ(result, 1.9965);

    ss.str("(1-2^(-24))*2^128");
    ss.clear();

    parse.read(ss);
    logSymbols(parse.symbols());
    result = eval.execute(parse.symbols());
    EXPECT_DOUBLE_EQ(result, (1 - pow(2, -24)) * pow(2, 128));
}

GTEST_TEST(Expression, Parse007)
{
    StringStream ss;
    ss <<"a=sin(x/2),b=4*atan(1)";

    Eq::StmtParser parse;
    parse.read(ss);
    logSymbols(parse.symbols());

    Eq::Stmt eval;
    eval.set("x", 3.1415926535897932384626433832795);
    eval.execute(parse.symbols());

    EXPECT_DOUBLE_EQ(eval.get("a"), 1.0);
    EXPECT_DOUBLE_EQ(eval.get("b"), 3.1415926535897932384626433832795);
}
///////////////////////////////////////////////////////////////////////////////

GTEST_TEST(Expression, Parse006)
{
    
    StringStream ss;
    ss <<"x={0,1,2,3}, y=[4,5,6,7], z={8,9,10,11}";

    Eq::StmtParser parse;
    parse.read(ss);

    const Eq::SymbolArray& exec = parse.symbols();
    logSymbols(exec);

    ExpectDataTest(exec, Parse6Values, exec.size());

    Eq::Stmt  eval;
    const R64 v = eval.execute(exec);
    EXPECT_EQ(v, Eq::InitialHash + 2);

    Eq::ValueList values[3] = {{}, {}, {}};

    eval.get("x", values[0]);
    EXPECT_EQ(eval.get("x"), Eq::InitialHash);

    eval.get("y", values[1]);
    EXPECT_EQ(eval.get("y"), Eq::InitialHash + 1);

    eval.get("z", values[2]);
    EXPECT_EQ(eval.get("z"), Eq::InitialHash + 2);

    int j = 0;
    for (int i = 0; i < 3; ++i)
    {
        EXPECT_EQ(values[i][0], j);
        EXPECT_EQ(values[i][1], j + 1);
        EXPECT_EQ(values[i][2], j + 2);
        EXPECT_EQ(values[i][3], j + 3);
        j += 4;
    }
}

///////////////////////////////////////////////////////////////////////////////

void TestDouble(R64 a, R64 b)
{
    if (std::isnan(a) && std::isnan(b))
        return;

    EXPECT_DOUBLE_EQ(b, a);
}

void Parse4ValueCheck(
    const Eq::SymbolArray& exec,
    Eq::Stmt&              eval,
    R64                    a,
    R64                    b,
    R64                    c,
    R64                    d,
    R64                    e,
    R64                    f,
    R64                    x,
    R64                    expected)
{
    eval.set("s1", a);
    eval.set("s2", b);
    eval.set("s3", c);
    eval.set("s4", d);
    eval.set("s5", e);
    eval.set("s6", f);
    eval.set("x", x);

    eval.execute(exec);

    // EXPECT_EQ(
    //     equals(
    //         eval.execute(exec), expected, 0.00001),
    //     true);

    EXPECT_EQ(
        equals(
            eval.get("a", 0x00), expected, 0.00001),
        true);
    EXPECT_EQ(
        equals(
            eval.get("b", 0x00), expected, 0.00001),
        true);
}

GTEST_TEST(Expression, Parse005)
{
    StringStream ss;
    ss << "# (2*6^2)/(7/(11*x-1)) with control coefficients\n"
        << "a=b=s1*(s2*2*6^2)/(s3*7/(s4*11*s5*x-s6*1))";

    Eq::StmtParser parse;
    parse.read(ss);
    const Eq::SymbolArray& exec = parse.symbols();
    logSymbols(exec);

    Eq::Stmt eval;
    Parse4ValueCheck(exec, eval, 1, 1, 1, 1, 1, 1, 1, 102.85714285714285714285714285714);
    Parse4ValueCheck(exec, eval, 1, 1, 1, 1, 1, 1, 11, 1234.2857142857142857142857142858);
    Parse4ValueCheck(exec, eval, 1, 1, 1, 1, 1, -11, 1, 226.28571428571428571428571428571);
    Parse4ValueCheck(exec, eval, 1, 1, 1, 1, 1, 11, 1, NAN);
    Parse4ValueCheck(exec, eval, 1, 1, 1, 1, 1, -7, 0, 72.0);
    Parse4ValueCheck(exec, eval, 1, 1, 1, 1, 1, 7, 0, -72.0);
    Parse4ValueCheck(exec, eval, 1, 1, -1, 1, 1, 7, 0, 72.0);
    Parse4ValueCheck(exec, eval, 1, -2, -1, 1, 1, 7, 0, -144.0);
    Parse4ValueCheck(exec, eval, -2, -2, -1, 1, 1, 7, 0, 288.0);
}

///////////////////////////////////////////////////////////////////////////////

GTEST_TEST(Expression, Parse004)
{
    StringStream ss;
    ss << "y = 7+2*2";

    Eq::StmtParser parse;
    parse.read(ss);
    const Eq::SymbolArray& exec = parse.symbols();
    EXPECT_EQ(exec.size(), 7);
    logSymbols(exec);

    Eq::Stmt eval;
    EXPECT_EQ(eval.execute(exec), 11);
    EXPECT_EQ(eval.get("y"), 11);
}

///////////////////////////////////////////////////////////////////////////////

GTEST_TEST(Expression, Parse003)
{
    StringStream ss;
    ss << "y = 7+2*2";

    Eq::StmtParser parse;
    parse.read(ss);
    const Eq::SymbolArray& exec = parse.symbols();
    logSymbols(exec);

    EXPECT_EQ(exec.size(), 7);
    ExpectDataTest(exec, Parse3Values, exec.size());
}

///////////////////////////////////////////////////////////////////////////////

GTEST_TEST(Expression, Parse002)
{
    StringStream ss;
    ss << "# Tests recursive assignment.." << std::endl;
    ss << "a=b=c=d=e=f=1";

    Eq::StmtParser parse;
    parse.read(ss);
    const Eq::SymbolArray& exec = parse.symbols();
    logSymbols(exec);

    EXPECT_EQ(exec.size(), 13);
    ExpectDataTest(exec, Parse2Values, exec.size());

    Eq::Stmt eval;
    EXPECT_EQ(eval.execute(exec), 1);
    EXPECT_EQ(eval.get("a", -1), 1);
    EXPECT_EQ(eval.get("b", -1), 1);
    EXPECT_EQ(eval.get("c", -1), 1);
    EXPECT_EQ(eval.get("d", -1), 1);
    EXPECT_EQ(eval.get("e", -1), 1);
    EXPECT_EQ(eval.get("f", -1), 1);
}

///////////////////////////////////////////////////////////////////////////////

GTEST_TEST(Expression, Parse001)
{
    StringStream ss;
    ss <<"y = [0,1,2,3]";

    Eq::StmtParser parse;
    parse.read(ss);
    const Eq::SymbolArray& exec = parse.symbols();

    logSymbols(exec);
    EXPECT_EQ(exec.size(), 8);
    ExpectDataTest(exec, Parse1Values, exec.size());

    Eq::Stmt eval;
    EXPECT_EQ(eval.execute(exec), Eq::InitialHash);

    Eq::ValueList v;
    eval.get("y", v);

    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[1], 1);
    EXPECT_EQ(v[2], 2);
    EXPECT_EQ(v[3], 3);
}

///////////////////////////////////////////////////////////////////////////////

GTEST_TEST(Expression, Scan2)
{
    const String fileName = GetTestFilePath("scan3.eq");

    InputFileStream fs;
    fs.open(fileName);
    EXPECT_TRUE(fs.is_open());

    Eq::StmtScanner sc;
    sc.attach(&fs, PathUtil(fileName));

    Eq::Token tok;
    int8_t    val = Eq::TOK_KW_ST;
    while (val != Eq::TOK_EOF)
    {
        sc.scan(tok);
        val = tok.type();
        if (val != Eq::TOK_EOF)
        {
            bool test = val >= Eq::TOK_KW_ST && val <= Eq::TOK_KW_EN;
            EXPECT_EQ(test, true);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////

GTEST_TEST(Expression, Scan1)
{
    const String fileName = GetTestFilePath("scan1.eq");

    InputFileStream fs;
    fs.open(fileName);
    EXPECT_TRUE(fs.is_open());

    Eq::StmtScanner sc;
    sc.attach(&fs, PathUtil(fileName));

    constexpr Eq::TokenType exp[] = {
        Eq::TOK_MUL,
        Eq::TOK_DIV,
        Eq::TOK_PLUS,
        Eq::TOK_MINUS,
        Eq::TOK_O_PAR,
        Eq::TOK_C_PAR,
        Eq::TOK_O_BRACKET,
        Eq::TOK_C_BRACKET,
        Eq::TOK_O_BRACE,
        Eq::TOK_C_BRACE,
        Eq::TOK_COMMA,
        Eq::TOK_PERIOD,
        Eq::TOK_EQUALS,
        Eq::TOK_POWS,
    };

    Eq::Token tok;
    for (auto type : exp)
    {
        sc.scan(tok);
        EXPECT_EQ(tok.type(), type);
    }
}

///////////////////////////////////////////////////////////////////////////////

GTEST_TEST(Expression, Scan0)
{
    const String Scan0 = GetTestFilePath("scan0.eq");

    InputFileStream fs;
    fs.open(Scan0);
    EXPECT_TRUE(fs.is_open());

    Eq::StmtScanner sc;
    sc.attach(&fs, PathUtil(Scan0));

    Eq::Token tok;
    sc.scan(tok);
    EXPECT_EQ(tok.type(), Eq::TOK_FLOAT);
    EXPECT_EQ(123, sc.real(tok.index()));

    sc.scan(tok);
    EXPECT_EQ(tok.type(), Eq::TOK_MUL);

    sc.scan(tok);
    EXPECT_EQ(tok.type(), Eq::TOK_FLOAT);
    EXPECT_EQ(987, sc.real(tok.index()));

    sc.scan(tok);
    EXPECT_EQ(tok.type(), Eq::TOK_EOF);
}

///////////////////////////////////////////////////////////////////////////////

void logSymbols(const Eq::SymbolArray& sym)
{
    for (const auto element : sym)
        element->print();
    Console::writeLine("");
}

void ExpectDataTest(const Eq::SymbolArray& actual,
                    const ExpectData*      expected,
                    size_t                 size)
{
    for (size_t i = 0; i < size; ++i)
    {
        EXPECT_EQ(actual.at(i)->type(), expected[i].type);

        if (expected[i].type == Eq::Identifier)
            EXPECT_EQ(actual.at(i)->name(), expected[i].name);
        else if (expected[i].type == Eq::Numerical)
            EXPECT_EQ(actual.at(i)->value(), expected[i].value);
    }
}
