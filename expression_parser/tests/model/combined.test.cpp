#include <gtest/gtest.h>
#include "parser.hpp"

TEST(combined, parse_3_minus_4_plus_5)
{
    Expression_Parser parser;
    auto result = parser.parse("3 -4 +5  ");
    ASSERT_TRUE(result);
    EXPECT_EQ(3-4+5, result.expression->evaluate());
}

TEST(combined, parse_minus_3_plus_4_minus_5)
{
    Expression_Parser parser;
    auto result = parser.parse("-3+4-5");
    ASSERT_TRUE(result);
    EXPECT_EQ(-3+4-5, result.expression->evaluate());
}

TEST(combined, parse_3_plus_5_mult_3)
{
    Expression_Parser parser;
    auto result = parser.parse("3+5*3");
    ASSERT_TRUE(result);
    EXPECT_EQ(3+5*3, result.expression->evaluate());
}

TEST(combined, parse_3_plus_5_mult_3_minus_3)
{
    Expression_Parser parser;
    auto result = parser.parse("3+5*3-3");
    ASSERT_TRUE(result);
    EXPECT_EQ(3+5*3-3, result.expression->evaluate());
}

TEST(combined, parse_3_minus_5_mult_3_plus_2)
{
    Expression_Parser parser;
    auto result = parser.parse("3-5*3+2");
    ASSERT_TRUE(result);
    EXPECT_EQ(3-5*3+2, result.expression->evaluate());
}

TEST(combined, parse_1_plus_3_less_5)
{
    Expression_Parser parser;
    auto result = parser.parse("1+3<5");
    ASSERT_TRUE(result);
    EXPECT_EQ(1+3<5, result.expression->evaluate());
}

TEST(combined, parse_2_plus_4_less_10_minus_2_equal_5_less_8)
{
    Expression_Parser parser;
    auto result = parser.parse("2+4<10-2==5<8");
    ASSERT_TRUE(result);
    EXPECT_EQ(2+4<10-2==5<8, result.expression->evaluate());
}

TEST(combined, parse_3_plus_5_div_4)
{
    Expression_Parser parser;
    auto result = parser.parse("3+5/4");
    ASSERT_TRUE(result);
    EXPECT_EQ(3+5/4, result.expression->evaluate());
}

TEST(combined, parse_empty_div_3_plus_5_div_4)
{
    Expression_Parser parser;
    auto result = parser.parse("/3+5/4");
    ASSERT_TRUE(result);
    EXPECT_EQ(0/3+5/4, result.expression->evaluate());
}

TEST(combined, parse_1_plus_2_xor_3_minus_5_mult_7_equals_8_or_9)
{
    Expression_Parser parser;
    auto result = parser.parse("/3+5/4");
    ASSERT_TRUE(result);
    EXPECT_EQ(1+2^3-5*7==8||9, result.expression->evaluate());
}


