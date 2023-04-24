#include <gtest/gtest.h>
#include "parser.hpp"

TEST(greaterthan, parse_1_greater_2)
{
    Expression_Parser parser;
    Parse_Result result = parser.parse("1>2");
    ASSERT_TRUE(result);
    EXPECT_EQ(1>2 , result.expression->evaluate());
}

TEST(greaterthan, parse_3_less_2)
{
    Expression_Parser parser;
    Parse_Result result = parser.parse("3>2");
    ASSERT_TRUE(result);
    EXPECT_EQ(3>2,  result.expression->evaluate());
}

TEST(greaterthan, parse_2_less_or_equal_2)
{
    Expression_Parser parser;
    Parse_Result result = parser.parse("2>=2");
    ASSERT_TRUE(result);
    EXPECT_EQ(2>=2,  result.expression->evaluate());
}

TEST(greaterthan, parse_3_less_or_equal_2)
{
    Expression_Parser parser;
    Parse_Result result = parser.parse("3>=2");
    ASSERT_TRUE(result);
    EXPECT_EQ(3>=2,  result.expression->evaluate());
}

TEST(greaterthan, parse_1_less_or_equal_2)
{
    Expression_Parser parser;
    Parse_Result result = parser.parse("1>=2");
    ASSERT_TRUE(result);
    EXPECT_EQ(1>=2,  result.expression->evaluate());
}