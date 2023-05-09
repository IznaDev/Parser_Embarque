#include "../../include/validator.hpp"
#include <gtest/gtest.h>

#define SCHEMA "../../../schema.json"

TEST(validJSON, validJSON)
{
    ifstream valid("../../../automation_code_builder/tests_automation/test_files/validJson.json");
    json json = json::parse(valid);
    valid.close();

    EXPECT_NO_THROW(validJson(SCHEMA, json));
}

TEST(validJSON, noTarget)
{
    ifstream valid("../../../automation_code_builder/tests_automation/test_files/noTarget.json");
    json json = json::parse(valid);
    valid.close();

    EXPECT_ANY_THROW(validJson(SCHEMA, json));
}

TEST(validJSON, noDevice)
{
    ifstream valid("../../../automation_code_builder/tests_automation/test_files/noDevice.json");
    json json = json::parse(valid);
    valid.close();

    EXPECT_ANY_THROW(validJson(SCHEMA, json));
}

TEST(validJSON, noID)
{
    ifstream valid("../../../automation_code_builder/tests_automation/test_files/noID.json");
    json json = json::parse(valid);
    valid.close();

    EXPECT_ANY_THROW(validJson(SCHEMA, json));
}

TEST(validJSON, noType)
{
    ifstream valid("../../../automation_code_builder/tests_automation/test_files/noType.json");
    json json = json::parse(valid);
    valid.close();

    EXPECT_ANY_THROW(validJson(SCHEMA, json));
}

TEST(validJSON, noBehaviors)
{
    ifstream valid("../../../automation_code_builder/tests_automation/test_files/noBehaviors.json");
    json json = json::parse(valid);
    valid.close();

    EXPECT_ANY_THROW(validJson(SCHEMA, json));
}

TEST(validJSON, noIf)
{
    ifstream valid("../../../automation_code_builder/tests_automation/test_files/noIf.json");
    json json = json::parse(valid);
    valid.close();

    EXPECT_ANY_THROW(validJson(SCHEMA, json));
}

TEST(validJSON, noThen)
{
    ifstream valid("../../../automation_code_builder/tests_automation/test_files/noThen.json");
    json json = json::parse(valid);
    valid.close();

    EXPECT_ANY_THROW(validJson(SCHEMA, json));
}

TEST(validJSON, noElse)
{
    ifstream valid("../../../automation_code_builder/tests_automation/test_files/noElse.json");
    json json = json::parse(valid);
    valid.close();

    EXPECT_ANY_THROW(validJson(SCHEMA, json));
}



