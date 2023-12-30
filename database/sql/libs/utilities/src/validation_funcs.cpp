#include <validation_funcs.h>

#include <consts.h>

bool ip_v4::isIpValid(const std::string &ip_v4)
{
    if (ip_v4.size() < consts::ip_v4::kMinSize || ip_v4.size() > consts::ip_v4::kMaxSize)
    {
        return false;
    }

    std::regex ip_regex(consts::ip_v4::kRegExp);

    return std::regex_match(ip_v4, ip_regex);
}

bool date::isDateValid(const std::string& date)
{
    std::regex date_regex(consts::date::kRegExp);

    if (!std::regex_match(date, date_regex))
    {
        return false; 
    }

    int32_t year, month, day;
    uint8_t dash;
    std::istringstream date_stream(date);
    date_stream >> year >> dash >> month >> dash >> day;

    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);

    return !(year < consts::date::kMinYear || 
             year > now->tm_year + consts::date::kMinYear ||
             month < 1 || 
             month > consts::date::kMaxMonth ||
             day < 1 || 
             day > consts::date::kMaxDays);
}