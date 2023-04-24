#include <gtest/gtest.h>
#include "parser.hpp"

TEST(affectation, parse_set_variable_5)
{
    Expression_Parser parser;
    MemoryDataContext mdc;
    mdc.add_or_set("var", 0);
    auto result = parser.parse("${var} = 5");
    ASSERT_TRUE(result);
    EXPECT_TRUE(result.expression->update(&mdc));
    EXPECT_EQ(5, mdc.evaluate("var"));
}

TEST(affectation, parse_increase_variable_5)
{
    Expression_Parser parser;
    MemoryDataContext mdc;
    mdc.add_or_set("var", 5);
    auto result = parser.parse("${var} += 5");
    ASSERT_TRUE(result);
    EXPECT_TRUE(result.expression->update(&mdc));
    EXPECT_EQ(10, mdc.evaluate("var"));
}

TEST(affectation, parse_decrease_variable_5)
{
    Expression_Parser parser;
    MemoryDataContext mdc;
    mdc.add_or_set("var", 5);
    auto result = parser.parse("${var} -= 5");
    ASSERT_TRUE(result);
    EXPECT_TRUE(result.expression->update(&mdc));
    EXPECT_EQ(0, mdc.evaluate("var"));
}