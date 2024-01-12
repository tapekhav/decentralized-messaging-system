#pragma once

class CacheManagerTests : public ::testing::Test
{
protected:
    void SetUp() override
    {
        _key_value = {1, std::string("value")};
        _manager_ptr = std::make_unique<CacheManager<int32_t, std::string>>(
            redis_db::consts::ip_v4, 
            redis_db::consts::port
        );

        _manager_ptr->createValue(_key_value);
    }
    
    std::unique_ptr<CacheManager<int32_t, std::string>> _manager_ptr;
    std::pair<int32_t, std::string> _key_value;
};