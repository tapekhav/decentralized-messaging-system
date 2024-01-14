#pragma once


class CachePairTests : public ::testing::Test
{
protected:
    void SetUp() override
    {

        _manager_ptr = std::make_unique<CacheManager<int32_t, std::string>>(
            redis_db::consts::ip_v4, 
            redis_db::consts::port
        );

        _key_value1 = {1, std::string("value1")};
        _key_value2 = {2, std::string("value2")};
        _manager_ptr->createValue(_key_value1);
        _manager_ptr->createValue(_key_value2);
    }
    
    std::unique_ptr<CacheManager<int32_t, std::string>> _manager_ptr;

    std::pair<int32_t, std::string> _key_value1;
    std::pair<int32_t, std::string> _key_value2;
};