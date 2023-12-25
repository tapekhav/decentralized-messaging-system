#pragma once

#include <base_soci_exception.h>

static const std::string what_string = "Incorrect number of arguments passed to the insert request (it should be 5)";

class InsertNumberException final : public SociBaseException
{
public:
    InsertNumberException() : SociBaseException(what_string) {}

    [[nodiscard]] const char* name() const noexcept final { return "InsertNumberException"; }
    
    ~InsertNumberException() noexcept final = default;
};