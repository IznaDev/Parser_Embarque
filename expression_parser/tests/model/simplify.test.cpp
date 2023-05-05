#include <gtest/gtest.h>
#include "parser.hpp"

TEST(simplify, parse_3_plus_5_minus_4)
{
    Expression_Parser parser;
    Parse_Result result = parser.parse("3+5-4");
    ASSERT_TRUE(result);
    Expression* simplified = result.expression->simplify();
    EXPECT_TRUE(simplified->is_leaf());
    EXPECT_EQ(3+5-4, result.expression->evaluate());
}