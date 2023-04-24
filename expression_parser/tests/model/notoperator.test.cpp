#include <gtest/gtest.h>
#include "parser.hpp"

TEST(addition, parse_not_1)
{
    Expression_Parser parser;
    auto result = parser.parse("!1");
    ASSERT_TRUE(result);
    EXPECT_EQ(!1, result.expression->evaluate());
}

TEST(addition, parse_not_0)
{
    Expression_Parser parser;
    auto result = parser.parse("!0");
    ASSERT_TRUE(result);
    EXPECT_EQ(!0, result.expression->evaluate());
}

TEST(addition, parse_not_1_less_2)
{
    Expression_Parser parser;
    auto result = parser.parse("!1<2");
    ASSERT_TRUE(result);
    EXPECT_EQ(!1<2, result.expression->evaluate());
}

TEST(addition, parse_not_par_1_less_2_par)
{
    Expression_Parser parser;
    auto result = parser.parse("!(1<2)");
    ASSERT_TRUE(result);
    EXPECT_EQ(!(1<2), result.expression->evaluate());
}