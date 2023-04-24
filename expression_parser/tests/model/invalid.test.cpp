#include <gtest/gtest.h>
#include "parser.hpp"

TEST(invalid, empty_expr)
{
    Expression_Parser parser;
    EXPECT_FALSE(parser.parse("  "));
}

TEST(invalid, not_supported_operator)
{
    Expression_Parser parser;
    EXPECT_FALSE(parser.parse("3 p 5"));
}

TEST(invalid, wrong_closing_parenthesis_number)
{
    Expression_Parser parser;
    EXPECT_FALSE(parser.parse("(3+5))"));
}

TEST(invalid, wrong_opening_parenthesis_number)
{
    Expression_Parser parser;
    EXPECT_FALSE(parser.parse("((3+5)"));
}

TEST(invalid, two_member_without_operator)
{
    Expression_Parser parser;
    EXPECT_FALSE(parser.parse("3 5"));
}

TEST(invalid, two_member_with_parenthesis_without_operator)
{
    Expression_Parser parser;
    EXPECT_FALSE(parser.parse("(3+5)(4)"));
}

TEST(invalid, 1_equ_1)
{
    Expression_Parser parser;
    EXPECT_FALSE(parser.parse("1=1"));
}

TEST(invalid, wrong_refer)
{
    Expression_Parser parser;
    string wrong_refs[]= {"${}", "${a b}", "${test", "${a..b}, $a b}"};
    for(const auto& s: wrong_refs)
    {
        EXPECT_FALSE(parser.parse(s));
    }
    
}

TEST(invalid, closing_parenthesis_without_opening)
{
    Expression_Parser parser;
    EXPECT_FALSE(parser.parse(")4("));
}

TEST(invalid, closing_parenthesis_without_opening_2)
{
    Expression_Parser parser;
    EXPECT_FALSE(parser.parse("(3+1)+)5("));
}