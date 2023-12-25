#pragma once

#include <string>
#include <cstdint>

namespace consts
{
    namespace db 
    {
        const std::size_t kNumOfInsertQueryArgs = 5;
    }

    namespace ip_v4 
    {
        const std::size_t kMinSize = 8;
        const std::size_t kMaxSize = 16;

        const std::string kRegExp = "^((25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)$";   
    }
};