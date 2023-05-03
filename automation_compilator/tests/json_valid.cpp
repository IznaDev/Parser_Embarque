#include <gtest/gtest.h>
#include "../include/validator.hpp"

TEST(json_valid, noError)
{
    EXPECT_NO_THROW(validJson("../../schema/schema.json", "../../schema_validator.json"));
}

TEST(json_valid, Error_NO_TARGET)
{
    EXPECT_ANY_THROW(validJson("../schema/schema.json", "test_file/no_target.json"));
}

TEST(json_valid, Error_NO_DEVICE)
{
    EXPECT_ANY_THROW(validJson("../schema/schema.json", "test_file/no_device.json"));
}

TEST(json_valid, Error_NO_BEHAVIOURS)
{
    EXPECT_ANY_THROW(validJson("../schema/schema.json", "test_file/no_behaviors.json"));
}

TEST(json_valid, Error_NO_ID)
{
    EXPECT_ANY_THROW(validJson("../schema/schema.json", "test_file/not_id.json"));
}

TEST(json_valid, Error_NO_TYPE)
{
    EXPECT_ANY_THROW(validJson("../schema/schema.json", "test_file/not_type.json"));
}

TEST(json_valid, Error_NOT_UNIQ_ITEM)
{
    EXPECT_ANY_THROW(validJson("../schema/schema.json", "test_file/not_uniq_item.json"));
}

TEST(json_valid, Error_NO_THEN)
{
    EXPECT_ANY_THROW(validJson("../schema/schema.json", "test_file/no_then.json"));
}

TEST(json_valid, Error_NO_ELSE)
{
    EXPECT_ANY_THROW(validJson("../schema/schema.json", "test_file/no_else.json"));
}

TEST(json_valid, Error_NO_IF)
{
    EXPECT_ANY_THROW(validJson("../schema/schema.json", "test_file/no_if.json"));
}


