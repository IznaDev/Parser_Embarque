#include <gtest/gtest.h>
#include "container.hpp"

TEST(container, const_char_start_long_dict)
{
    Dictionary<const char*,long> dict;
    dict["test"] = 10;
    EXPECT_EQ(10, dict["test"]);
    EXPECT_EQ(1, dict.size());
    dict["test2"]=20;
    dict["atest"]=0;
    EXPECT_EQ(3, dict.size());
    EXPECT_EQ(20, dict["test2"]);
    EXPECT_EQ(0, dict["atest"]);
    dict["atfest"]=5;
    ASSERT_EQ(4, dict.size());
    EXPECT_EQ("atest", dict.get_key(0));
    EXPECT_EQ("atfest", dict.get_key(1));
    EXPECT_EQ("test", dict.get_key(2));
    EXPECT_EQ("test2", dict.get_key(3));
    dict["test"] = dict["atfest"];
    EXPECT_EQ(5, dict["test"]);
    EXPECT_TRUE(dict.remove("test"));
    EXPECT_EQ(3, dict.size());
    EXPECT_FALSE(dict.remove("dummy"));
}

TEST(container, string_int_dict)
{
    Dictionary<std::string, int> dict;
    dict["test"] = 10;
    EXPECT_EQ(10, dict["test"]);
    EXPECT_EQ(1, dict.size());
    dict["test2"]=20;
    dict["atest"]=0;
    EXPECT_EQ(3, dict.size());
    EXPECT_EQ(20, dict["test2"]);
    EXPECT_EQ(0, dict["atest"]);
    dict["atfest"]=5;
    ASSERT_EQ(4, dict.size());
    EXPECT_EQ("atest", dict.get_key(0));
    EXPECT_EQ("atfest", dict.get_key(1));
    EXPECT_EQ("test", dict.get_key(2));
    EXPECT_EQ("test2", dict.get_key(3));
    dict["test"] = dict["atfest"];
    EXPECT_EQ(5, dict["test"]);
    EXPECT_TRUE(dict.remove("test"));
    EXPECT_EQ(3, dict.size());
    EXPECT_FALSE(dict.remove("dummy"));
}