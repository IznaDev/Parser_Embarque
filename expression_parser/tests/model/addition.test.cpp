#include <gtest/gtest.h>
#include "parser.hpp"

TEST(addition, parse_const_34)
{
    Expression_Parser parser;
    auto result = parser.parse("34");
    ASSERT_TRUE(result);
    EXPECT_EQ(34, result.expression->evaluate());
}

TEST(addition, parse_const_3_plus_4)
{
    Expression_Parser parser;
    auto result = parser.parse("3+4");
    ASSERT_TRUE(result);
    EXPECT_EQ(3+4, result.expression->evaluate());
}

TEST(addition, parse_const_3_plus_4_with_spaces)
{
    Expression_Parser parser;
    auto result = parser.parse("3 +\t4");
    ASSERT_TRUE(result);
    EXPECT_EQ(3+4,result.expression->evaluate());
}

TEST(addition, parse_const_3_plus_4_plus_5)
{
    Expression_Parser parser;
    auto result = parser.parse("3 + 4 + 5");
    ASSERT_TRUE(result);
    EXPECT_EQ(3+4+5, result.expression->evaluate());
}

TEST(addition, parse_const_1_plus_2_plus_3_plus_4)
{
    Expression_Parser parser;
    auto result = parser.parse("1+2 + 3+4");
    ASSERT_TRUE(result);
    EXPECT_EQ(1+2+3+4, result.expression->evaluate());
}

TEST(addition, parse_const_empty_plus_3)
{
    Expression_Parser parser;
    auto result = parser.parse("+3");
    ASSERT_TRUE(result);
    EXPECT_EQ(+3, parser.parse("+3").expression->evaluate());
}

TEST(addition, parse_const_space_plus_3)
{
    Expression_Parser parser;
    EXPECT_EQ(3, parser.parse(" \t+ 3").expression->evaluate()); 
}

TEST(addition, parse_const_3_plus_empty)
{
    Expression_Parser parser;
    EXPECT_EQ(3, parser.parse("3+").expression->evaluate()); 
}

TEST(addition, parse_const_3_plus_empty_plus_4)
{
    Expression_Parser parser;
    EXPECT_EQ(3+4, parser.parse("3+ + 4").expression->evaluate()); 
}

TEST(addition, parse_const_empty_plus_3_plus_4)
{
    Expression_Parser parser;
    EXPECT_EQ(3+4, parser.parse("+3+4").expression->evaluate()); 
}

TEST(addition, parse_const_3_plus_4_plus_empty)
{
    Expression_Parser parser;
    EXPECT_EQ(3+4, parser.parse("3+4+").expression->evaluate());
}