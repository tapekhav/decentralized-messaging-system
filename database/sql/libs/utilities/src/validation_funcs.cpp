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