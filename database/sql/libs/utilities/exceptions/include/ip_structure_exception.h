#pragma once

#include <base_soci_exception.h>

static const std::string kIpError = "Incorrect structure of ip_v4 passed to the request";

class IpStructureException final : public SociBaseException
{
public:
    IpStructureException() : SociBaseException(kIpError) {}

    [[nodiscard]] const char* name() const noexcept final { return "IpStructureException"; }
    
    ~IpStructureException() noexcept final = default;
};