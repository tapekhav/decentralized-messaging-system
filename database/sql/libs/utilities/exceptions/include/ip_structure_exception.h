#pragma once

#include <base_exception.h>

static const std::string kIpError = "Incorrect structure of ip_v4 passed to the request";

class IpStructureException final : public BaseException
{
public:
    IpStructureException() : BaseException(kIpError) {}

    [[nodiscard]] const char* name() const noexcept final { return "IpStructureException"; }
    
    ~IpStructureException() noexcept final = default;
};