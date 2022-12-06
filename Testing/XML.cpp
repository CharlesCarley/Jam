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
#include "Utils/FileSystem.h"
#include "Xml/File.h"
#include "gtest/gtest.h"

using namespace Jam;

void GenTag(StringStream& ss, int c, int max)
{
    if (c > max)
        return;
    ss << "<t" << c << ">";
    GenTag(ss, c + 1, max);
    ss << "</t" << c << ">";
}

GTEST_TEST(XmlParse, MaxTest)
{
    StringStream ss;
    GenTag(ss, 0, 100);
    try
    {
        Xml::File parser(99);
        parser.read(ss);

        constexpr bool threwException = false;
        EXPECT_TRUE(threwException);
    }
    catch (...)
    {
        EXPECT_TRUE(true);
    }
}

void logNode(OStream& out, Xml::Node* node, int& depth)
{
    for (int i = 0; i < depth; ++i)
        out << '-';
    out << node->name() << std::endl;

    for (size_t i = 0; i < node->size(); ++i)
    {
        Xml::Node* ch = node->at(i);

        ++depth;
        logNode(out, ch, depth);
        --depth;
    }
}

GTEST_TEST(XmlParse, TextStructure)
{
    StringStream ss;
    ss << "<R>";
    ss << "<T0>TextNode1";
    ss << "<T1>TextNode2";
    ss << "</T1>TextNode3";
    ss << "</T0>";
    ss << "</R>";
    Xml::File parser;
    parser.read(ss);
    Xml::Node* root = parser.root("R");

    OutputStringStream oss;
    int                d = 1;
    logNode(oss, root, d);

    StringStream expected;
    expected << "-R" << std::endl;
    expected << "--T0" << std::endl;
    expected << "---_text_node" << std::endl;
    expected << "---T1" << std::endl;
    expected << "----_text_node" << std::endl;
    expected << "---_text_node" << std::endl;
    EXPECT_EQ(expected.str(), oss.str());

    Xml::Node* t0  = root->firstChild();
    Xml::Node* tn1 = t0->at(0);
    Xml::Node* t1  = t0->at(1);
    Xml::Node* tn2 = t1->at(0);
    Xml::Node* tn3 = t0->at(2);

    EXPECT_EQ("TextNode1", tn1->text());
    EXPECT_EQ("TextNode2", tn2->text());
    EXPECT_EQ("TextNode3", tn3->text());
}
