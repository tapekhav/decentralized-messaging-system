#pragma once

#include <regex>

namespace ip_v4
{
    auto isIpValid(const std::string& ip_v4) -> bool; 
}

namespace date
{
    auto isDateValid(const std::string& date) -> bool;
}