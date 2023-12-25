#pragma once

#include <base_soci_exception.h>

static const std::string what_string = "Incorrect structure of ip_v4 passed to the request";

class IpStructureException final : public SociBaseException
{
public:
    IpStructureException() : SociBaseException(what_string) {}

    [[nodiscard]] const char* name() const noexcept final { return "IpStructureException"; }
    
    ~IpStructureException() noexcept final = default;
};