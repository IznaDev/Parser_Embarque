#include <gtest/gtest.h>
#include "../include/validator.hpp"

TEST(schema_create, IsCreated)
{
    json emptySchema;
    EXPECT_NE(createSchema("../../schema_validator.json"), emptySchema);
}