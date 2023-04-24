#include <gtest/gtest.h>
#include "parser.hpp"

TEST(division, parse_const_3_div_4)
{
    Expression_Parser parser;
    EXPECT_EQ(3/4, parser.parse("3/4").expression->evaluate());
}

TEST(division, parse_const_3_div_4_with_spaces)
{
    Expression_Parser parser;
    EXPECT_EQ(3/4,parser.parse(" 3 /\t4").expression->evaluate());
}

TEST(division, parse_const_3_div_4_div_5)
{
    Expression_Parser parser;
    EXPECT_EQ(3/4/5, parser.parse("3 / 4 / 5").expression->evaluate());
}

TEST(division, parse_const_1_div_2_div_3_div_4)
{
    Expression_Parser parser;
    EXPECT_EQ(1/2/3/4, parser.parse("1/2 / 3/4").expression->evaluate());
}

TEST(division, parse_const_space_div_3)
{
    Expression_Parser parser;
    EXPECT_EQ(0/3, parser.parse(" \t/ 3").expression->evaluate()); 
}

TEST(division, parse_const_3_div_empty)
{
    Expression_Parser parser;
    EXPECT_EQ(3/1, parser.parse("3/").expression->evaluate()); 
}

TEST(division, parse_const_3_div_empty_div_4)
{
    Expression_Parser parser;
    EXPECT_EQ(3/1/4, parser.parse("3/ / 4").expression->evaluate()); 
}