#include <gtest/gtest.h>
#include "parser.hpp"

TEST(and, parse_1_and_0)
{
    Expression_Parser parser;
    auto result = parser.parse("1 and 0");
    ASSERT_TRUE(result);
    EXPECT_EQ(1&&0 , result.expression->evaluate());
}

TEST(and, parse_1_and_caps_0)
{
    Expression_Parser parser;
    auto result = parser.parse("1 AND 0");
    ASSERT_TRUE(result);
    EXPECT_EQ(1&&0 , result.expression->evaluate());
}

TEST(and, parse_1_and_1)
{
    Expression_Parser parser;
    auto result = parser.parse("1 and 1");
    ASSERT_TRUE(result);
    EXPECT_EQ(1&&1 , result.expression->evaluate());
}

TEST(and, parse_1_and_caps_1)
{
    Expression_Parser parser;
    auto result = parser.parse("1 AND 1");
    ASSERT_TRUE(result);
    EXPECT_EQ(1&&1 , result.expression->evaluate());
}

TEST(and, parse_0_and_0)
{
    Expression_Parser parser;
    auto result = parser.parse("0 and 0");
    ASSERT_TRUE(result);
    EXPECT_EQ(0&&0 , result.expression->evaluate());
}

TEST(and, parse_0_and_caps_0)
{
    Expression_Parser parser;
    auto result = parser.parse("0 AND 0");
    ASSERT_TRUE(result);
    EXPECT_EQ(0&&0 , result.expression->evaluate());
}

TEST(or, parse_1_or_0)
{
    Expression_Parser parser;
    auto result = parser.parse("1 or 0");
    ASSERT_TRUE(result);
    EXPECT_EQ(1||0 , result.expression->evaluate());
}

TEST(or, parse_1_or_caps_0)
{
    Expression_Parser parser;
    auto result = parser.parse("1 OR 0");
    ASSERT_TRUE(result);
    EXPECT_EQ(1||0 , result.expression->evaluate());
}

TEST(or, parse_1_or_1)
{
    Expression_Parser parser;
    auto result = parser.parse("1 or 1");
    ASSERT_TRUE(result);
    EXPECT_EQ(1||1 , result.expression->evaluate());
}

TEST(or, parse_1_or_caps_1)
{
    Expression_Parser parser;
    auto result = parser.parse("1 OR 1");
    ASSERT_TRUE(result);
    EXPECT_EQ(1||1 , result.expression->evaluate());
}

TEST(or, parse_0_or_0)
{
    Expression_Parser parser;
    auto result = parser.parse("0 or 0");
    ASSERT_TRUE(result);
    EXPECT_EQ(0||0 , result.expression->evaluate());
}

TEST(or, parse_0_or_caps_0)
{
    Expression_Parser parser;
    auto result = parser.parse("0 OR 0");
    ASSERT_TRUE(result);
    EXPECT_EQ(0||0 , result.expression->evaluate());
}