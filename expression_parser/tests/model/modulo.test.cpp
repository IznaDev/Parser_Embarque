#include <gtest/gtest.h>
#include "parser.hpp"

TEST(modulo, parse_const_3_mod_4)
{
    Expression_Parser parser;
    EXPECT_EQ(10%3, parser.parse("10%3").expression->evaluate());
}