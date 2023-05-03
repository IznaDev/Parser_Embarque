#include <gtest/gtest.h>
#include "../include/validator.hpp"

TEST(json_correct, trueAssertionJSON1)
{
    EXPECT_EQ(isCorrect("../../tests/test_file/test.json"), true);
}

TEST(json_correct, trueAssertionJSON2)
{
    EXPECT_EQ(isCorrect("../../tests/test_file/test2.json"), true);
}

TEST(json_correct, falseAssertionEMPTY)
{
    EXPECT_ANY_THROW(isCorrect("../../tests/test_file/test3.json"));
}

TEST(json_correct, falseAssertionHPP)
{
    EXPECT_ANY_THROW(isCorrect("../../tests/test_file/test.hpp"));
}

TEST(json_correct, falseAssertionSTL)
{
    EXPECT_ANY_THROW(isCorrect("../../tests/test_file/test.stl"));
}

TEST(json_correct, falseAssertionTXT)
{
    EXPECT_ANY_THROW(isCorrect("../../tests/test_file/test.txt"));
}

