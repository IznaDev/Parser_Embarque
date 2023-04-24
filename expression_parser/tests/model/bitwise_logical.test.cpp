#include <gtest/gtest.h>
#include "parser.hpp"

TEST(bitwise_logical, parse_const_3_xor_4)
{
    Expression_Parser parser;
    auto result = parser.parse("3^4");
    ASSERT_TRUE(result);
    EXPECT_EQ(3^4, result.expression->evaluate());
}

TEST(bitwise_logical, parse_const_4_xor_7)
{
    Expression_Parser parser;
    auto result = parser.parse("4^7");
    ASSERT_TRUE(result);
    EXPECT_EQ(4^7, result.expression->evaluate());
}

TEST(bitwise_logical, parse_const_3_or_4)
{
    Expression_Parser parser;
    auto result = parser.parse("3|4");
    ASSERT_TRUE(result);
    EXPECT_EQ(3|4, result.expression->evaluate());
}

TEST(bitwise_logical, parse_const_4_and_7)
{
    Expression_Parser parser;
    auto result = parser.parse("4&7");
    ASSERT_TRUE(result);
    EXPECT_EQ(4&7, result.expression->evaluate());
}