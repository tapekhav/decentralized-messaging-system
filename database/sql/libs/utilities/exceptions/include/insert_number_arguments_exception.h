#pragma once

#include <base_soci_exception.h>

static const std::string kNumberArgsError = "Incorrect number of arguments passed to the insert request (it should be 5)";

class InsertNumberException final : public SociBaseException
{
public:
    InsertNumberException() : SociBaseException(kNumberArgsError) {}

    [[nodiscard]] const char* name() const noexcept final { return "InsertNumberException"; }
    
    ~InsertNumberException() noexcept final = default;
};