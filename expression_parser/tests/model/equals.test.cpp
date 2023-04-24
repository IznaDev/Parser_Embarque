#include <gtest/gtest.h>
#include "parser.hpp"

TEST(equals, parse_1_equals_1)
{
    Expression_Parser parser;
    auto result = parser.parse("1==1");
    ASSERT_TRUE(result);    
    EXPECT_EQ(1==1, result.expression->evaluate());
}

TEST(equals, parse_1_equals_2)
{
    Expression_Parser parser;
    auto result = parser.parse("1==2");
    ASSERT_TRUE(result);    
    EXPECT_EQ(1==2, result.expression->evaluate());
}

TEST(equals, parse_1_not_equals_1)
{
    Expression_Parser parser;
    auto result = parser.parse("1!=1");
    ASSERT_TRUE(result);    
    EXPECT_EQ(1!=1, result.expression->evaluate());
}

TEST(equals, parse_1_not_equals_2)
{
    Expression_Parser parser;
    auto result = parser.parse("1!=2");
    ASSERT_TRUE(result);    
    EXPECT_EQ(1!=2, result.expression->evaluate());
}