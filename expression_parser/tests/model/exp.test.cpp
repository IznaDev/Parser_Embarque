#include <gtest/gtest.h>
#include "parser.hpp"

TEST(exp, parse_const_3_exp_4)
{
    Expression_Parser parser;
    auto result = parser.parse("pow(3,4)");
    ASSERT_TRUE(result);
    EXPECT_EQ(pow(3,4), result.expression->evaluate());
}

TEST(exp, parse_const_2_exp_33)
{
    Expression_Parser parser;
    auto result = parser.parse("pow(2,33)");
    ASSERT_TRUE(result);
    EXPECT_EQ(pow(2,33), result.expression->evaluate());
}