#include <gtest/gtest.h>
#include "parser.hpp"

TEST(parenthesis, parse_const_3_minus_parenthesis_4_plus_5_parenthesis)
{
    Expression_Parser parser;
    const auto result = parser.parse("3 - (4+5)");
    ASSERT_TRUE(result);
    EXPECT_EQ(3-(4+5), result.expression->evaluate());
}

TEST(parenthesis, parse_parenthesis_4_parenthesis)
{
    Expression_Parser parser;
    const auto result = parser.parse("(4)");
    ASSERT_TRUE(result);
    EXPECT_EQ((4), result.expression->evaluate());
}

TEST(parenthesis, parse_par_minus_3_plus_2_par_minus_par_plus_par_3_minus_2_par)
{
    Expression_Parser parser;
    const auto result = parser.parse("(-3+2)+(3-2)");
    ASSERT_TRUE(result);
    EXPECT_EQ((-3+2)+(3-2), result.expression->evaluate());
}

TEST(parenthesis, parse_par_minus_3_plus_4_minus_par_5_plus_2_par)
{
    Expression_Parser parser;
    const auto result = parser.parse("3+4-(5+2)");
    ASSERT_TRUE(result.expression);
    EXPECT_EQ(3+4-(5+2), result.expression->evaluate());
}

TEST(parenthesis, parse_par_3_plus_5_par_mult_par_5_minus_3_par)
{
    Expression_Parser parser;
    const auto result = parser.parse("(3+5)*(5-3)");
    ASSERT_TRUE(result.expression);
    EXPECT_EQ((3+5)*(5-3), result.expression->evaluate());
}

TEST(parenthesis, parse_1_plus_2_mult_par_3_minus_2_par_mult_6)
{
    Expression_Parser parser;
    const auto result = parser.parse("1+2*(3-2)*6");
    ASSERT_TRUE(result.expression);
    EXPECT_EQ(1+2*(3-2)*6, result.expression->evaluate());
}

TEST(parenthesis, parse_par_par_3_plus_1_par_mult_par_5_minus_2_par_par)
{
    Expression_Parser parser;
    const auto result = parser.parse("((3+1)*(5-2))");
    ASSERT_TRUE(result.expression);
    EXPECT_EQ(((3+1)*(5-2)), result.expression->evaluate());
}

TEST(parenthesis, parse_empty_minus_minus_3)
{
    Expression_Parser parser;
    EXPECT_EQ(-(-3), parser.parse("-(-3)").expression->evaluate());
}

TEST(parenthesis, parse_3_div_par_4_mult_par_5_plus_1)
{
    Expression_Parser parser;
    EXPECT_EQ((3/(4*5))+1, parser.parse("(3/(4*5))+1").expression->evaluate());
}



