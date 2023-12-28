#pragma once

#include <cstddef>
#include <string>
#include <cstdint>

namespace consts
{
    namespace db 
    {
        const std::string kUri = "dbname=users user=postgres password=1234 host=127.0.0.1 port=5433";
        const std::size_t kNumOfAllColumns = 7;
        const std::size_t kNumOfUsersColumns = 2;
        const std::size_t kNumOfDataArgs = 5;
        const std::size_t kMaxAddInfoSize = 255;
        const std::size_t kMaxNameSize = 60;
    }

    namespace ip_v4 
    {
        const std::size_t kMinSize = 8;
        const std::size_t kMaxSize = 16;

        const std::string kRegExp = "^((25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)$";   
    }

    namespace date
    {
        const int32_t kMaxDays = 31;
        const int32_t kMinYear = 1900;
        const int32_t kMaxMonth = 12; 

        const std::string kRegExp = R"(\b\d{4}-\d{2}-\d{2}\b)";
    }
};