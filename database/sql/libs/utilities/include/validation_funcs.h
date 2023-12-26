#pragma once

#include <regex>

namespace ip_v4
{
    bool isIpValid(const std::string& ip_v4); 
}

namespace date
{
    bool isDateValid(const std::string& date);
}