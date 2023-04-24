#include <gtest/gtest.h>
#include "parser.hpp"


TEST(multiplication, parse_const_3_mult_4)
{
    Expression_Parser parser;
    EXPECT_EQ(3*4, parser.parse("3*4").expression->evaluate());
}

TEST(multiplication, parse_const_3_mult_4_with_spaces)
{
    Expression_Parser parser;
    EXPECT_EQ(3*4,parser.parse(" 3 *\t4").expression->evaluate());
}

TEST(multiplication, parse_const_3_mult_4_mult_5)
{
    Expression_Parser parser;
    EXPECT_EQ(3*4*5, parser.parse("3 * 4 * 5").expression->evaluate());
}

TEST(multiplication, parse_const_1_mult_2_mult_3_mult_4)
{
    Expression_Parser parser;
    EXPECT_EQ(1*2*3*4, parser.parse("1*2 * 3*4").expression->evaluate());
}

TEST(multiplication, parse_const_space_mult_3)
{
    Expression_Parser parser;
    EXPECT_EQ(0*3, parser.parse(" \t* 3").expression->evaluate()); 
}

TEST(multiplication, parse_const_3_mult_empty)
{
    Expression_Parser parser;
    EXPECT_EQ(3*0, parser.parse("3*").expression->evaluate()); 
}

TEST(multiplication, parse_const_3_mult_empty_mult_4)
{
    Expression_Parser parser;
    EXPECT_EQ(3*0*4, parser.parse("3* * 4").expression->evaluate()); 
}