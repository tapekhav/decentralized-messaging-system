#include <iostream>

#include <gtest/gtest.h>

#include <cache_manager_test_fixture.h>

TEST_F(CacheManagerTests, CreateValueAndKeyExists) 
{
    std::pair<int32_t, std::string> key_value(1, "\"value1\"");
    _manager_ptr->createValue(key_value);

    EXPECT_TRUE(_manager_ptr->keyExists(key_value.first));
}

TEST_F(CacheManagerTests, ReadValue) 
{
    std::pair<int32_t, std::string> key_value(2, "value2");
    _manager_ptr->createValue(key_value);

    auto result = "\"" + key_value.second + "\"";
    EXPECT_EQ(_manager_ptr->readValue(key_value.first), result);
}

TEST_F(CacheManagerTests, ReadKeyByValue)
{
    std::pair<int32_t, std::string> key_value(3, "value3");
    _manager_ptr->createValue(key_value);

    auto result = _manager_ptr->readKeyByValue(key_value.second);

    if (result)
    {
        EXPECT_EQ(*result, key_value.first);
    } 
    else 
    {
        FAIL() << "Key not found for value: " << key_value.second;
    }
}
