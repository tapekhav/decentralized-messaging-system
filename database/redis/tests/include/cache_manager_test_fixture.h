#pragma once

#include <cache_manager.h>

class CacheManagerTests : public ::testing::Test
{
protected:
    void SetUp() override
    {
        _manager_ptr = std::make_unique<CacheManager<int32_t, std::string>>(
            redis_db::consts::ip_v4, 
            redis_db::consts::port
        );
    }
    
    std::unique_ptr<CacheManager<int32_t, std::string>> _manager_ptr;
};