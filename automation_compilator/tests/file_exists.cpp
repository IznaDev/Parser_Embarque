#include <gtest/gtest.h>
#include "../include/validator.hpp"

TEST(file_exist, trueAssertions1){
    EXPECT_EQ(isExist("../../schema_validator.json"), true);
}

TEST(file_exist, trueAssertions2){
    EXPECT_EQ(isExist("../../schema/schema.json"), true);
}

TEST(file_exist, trueAssertions3){
    EXPECT_EQ(isExist("../../tests/test_file/test.json"), true);
}

TEST(file_exist, trueAssertions4){
    EXPECT_EQ(isExist("../../tests/test_file/no_behaviors.json"), true);
}

TEST(file_exists, falseAssertions1){
    EXPECT_EQ(isExist("../schema_validator"), false);
}

TEST(file_exists, falseAssertions2){
    EXPECT_EQ(isExist("../ààà"), false);
}

