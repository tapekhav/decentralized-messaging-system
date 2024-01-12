#include <iostream>

#include <gtest/gtest.h>

#include <cache_manager.h>
#include <cache_pair_test_fixture.h>
#include <cache_manager_test_fixture.h>

TEST_F(CacheManagerTests, CreateValueAndKeyExists) 
{
    EXPECT_TRUE(_manager_ptr->keyExists(_key_value.first));
}

TEST_F(CacheManagerTests, ReadValue) 
{
    EXPECT_EQ(_manager_ptr->readValue(_key_value.first), _key_value.second);
}

TEST_F(CacheManagerTests, ReadKeyByValue) 
{
    auto result = _manager_ptr->readKeyByValue(_key_value.second);

    if (result)
    {
        EXPECT_EQ(result.value(), _key_value.first);
    } 
    else 
    {
        FAIL() << "Key not found for value: " << _key_value.second;
    }
}

TEST_F(CacheManagerTests, UpdateValue) 
{
    std::string updated_value = "updated_value4";
    _manager_ptr->updateValue({_key_value.first, updated_value});

    EXPECT_EQ(_manager_ptr->readValue(_key_value.first), updated_value);
}

TEST_F(CacheManagerTests, DeleteByKey) 
{
    _manager_ptr->deleteByKey(_key_value.first);

    EXPECT_FALSE(_manager_ptr->keyExists(_key_value.first));
}

TEST_F(CacheManagerTests, ClearCache) 
{
    _manager_ptr->clearCache();

    EXPECT_EQ(_manager_ptr->getCacheSize(), 0);
    EXPECT_FALSE(_manager_ptr->keyExists(_key_value.first));
}

TEST_F(CachePairTests, GetAllKeys) 
{
    auto keys = _manager_ptr->getAllKeys();

    EXPECT_TRUE(std::find(
        keys.begin(), 
        keys.end(), 
        _key_value1.first) != keys.end());
    EXPECT_TRUE(std::find(
        keys.begin(), 
        keys.end(), 
        _key_value2.first) != keys.end());
}

TEST_F(CachePairTests, GetAllValues) 
{
    auto values = _manager_ptr->getAllValues();

    EXPECT_TRUE(std::find(
        values.begin(), 
        values.end(), 
        _key_value1.second) != values.end());
    EXPECT_TRUE(std::find(
        values.begin(), 
        values.end(), 
        _key_value2.second) != values.end());
}

TEST_F(CachePairTests, GetAllKeyValuePairs) 
{
    auto key_value_pairs = _manager_ptr->getAllKeyValuePairs();

    EXPECT_TRUE(std::find(
        key_value_pairs.begin(), 
        key_value_pairs.end(), 
        _key_value1
    ) != key_value_pairs.end());
    EXPECT_TRUE(std::find(
        key_value_pairs.begin(), 
        key_value_pairs.end(), 
        _key_value2
    ) != key_value_pairs.end());
}

TEST_F(CacheManagerTests, HsetValueAndHgetValue)
{
    std::tuple<int32_t, std::string, std::string> hash_key_value(
            13, 
            std::string("field13"), 
            std::string("value13")
    );
    auto [key, field, val] = hash_key_value;
    _manager_ptr->hsetValue(key, field, val);
    
    _manager_ptr->deleteByKey(key);

    _manager_ptr->hsetValue(key, field, val);

    auto result = _manager_ptr->hgetValue(key, field);
    EXPECT_EQ(result, val);
}

TEST_F(CacheManagerTests, HkeyExists)
{
    std::tuple<int32_t, std::string, std::string> hash_key_value(14, "field14", "value14");

    auto [key, field, val] = hash_key_value;
    _manager_ptr->hsetValue(key, field, val);

    bool result = _manager_ptr->hkeyExists(key, field);
    EXPECT_TRUE(result);
}

TEST_F(CacheManagerTests, HdeleteKey)
{
    std::tuple<int32_t, std::string, std::string> hash_key_value(
        15, 
        std::string("field15"), 
        std::string("value15")
    );

    auto [key, field, val] = hash_key_value;
    _manager_ptr->hsetValue(key, field, val);

    _manager_ptr->hdeleteKey(key, field);

    bool result = _manager_ptr->hkeyExists(key, field);
    EXPECT_FALSE(result);
}
