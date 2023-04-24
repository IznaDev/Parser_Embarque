#include <gtest/gtest.h>
#include "parser.hpp"

TEST(shift, 1_shift_left_1)
{
    Expression_Parser parser;
    Parse_Result result = parser.parse("1<<1");
    ASSERT_TRUE(result);
    EXPECT_EQ(1<<1, result.expression->evaluate());
}

TEST(shift, 4_shift_right_2)
{
    Expression_Parser parser;
    Parse_Result result = parser.parse("4>>2");
    ASSERT_TRUE(result);
    EXPECT_EQ(4>>2, result.expression->evaluate());
}