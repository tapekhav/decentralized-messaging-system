#pragma once

#include <base_exception.h>

static const std::string kIpError = "Incorrect structure of ip_v4 passed to the request";

class IpStructureException final : public BaseException
{
public:
    IpStructureException() : BaseException(kIpError) {}
    IpStructureException(const IpStructureException& other) = delete;
    IpStructureException(IpStructureException&& other) = delete;

    auto operator=(const IpStructureException& other) -> IpStructureException& = delete;
    auto operator=(IpStructureException&& other) -> IpStructureException& = delete;

    [[nodiscard]] auto name() const noexcept -> const char* final
                                                         { return "IpStructureException"; }
    
    ~IpStructureException() noexcept final = default;
};