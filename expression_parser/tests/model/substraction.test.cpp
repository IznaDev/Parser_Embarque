#include <gtest/gtest.h>
#include "parser.hpp"

TEST(substraction, parse_const_34)
{
    Expression_Parser parser;
    EXPECT_EQ(34, parser.parse("34").expression->evaluate());
}

TEST(substraction, parse_const_3_minus_4)
{
    Expression_Parser parser;
    EXPECT_EQ(3-4, parser.parse("3-4").expression->evaluate());
}

TEST(substraction, parse_const_3_minus_4_with_spaces)
{
    Expression_Parser parser;
    EXPECT_EQ(3-4,parser.parse(" 3 -\t4").expression->evaluate());
}

TEST(substraction, parse_const_3_minus_4_minus_5)
{
    Expression_Parser parser;
    EXPECT_EQ(3-4-5, parser.parse("3 - 4 - 5").expression->evaluate());
}

TEST(substraction, parse_const_1_minus_2_minus_3_minus_4)
{
    Expression_Parser parser;
    EXPECT_EQ(1-2-3-4, parser.parse("1-2 - 3-4").expression->evaluate());
}

TEST(substraction, parse_const_empty_minus_3)
{
    Expression_Parser parser;
    EXPECT_EQ(-3, parser.parse("-3").expression->evaluate());
}

TEST(substraction, parse_const_space_minus_3)
{
    Expression_Parser parser;
    EXPECT_EQ(-3, parser.parse(" \t- 3").expression->evaluate()); 
}

TEST(substraction, parse_const_3_minus_empty)
{
    Expression_Parser parser;
    EXPECT_EQ(3, parser.parse("3-").expression->evaluate()); 
}

TEST(substraction, parse_const_3_minus_empty_minus_4)
{
    Expression_Parser parser;
    EXPECT_EQ(3-4, parser.parse("3- - 4").expression->evaluate()); 
}
