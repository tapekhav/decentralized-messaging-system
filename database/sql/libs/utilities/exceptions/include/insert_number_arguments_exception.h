#pragma once

#include <base_exception.h>

static const std::string kNumberArgsError = "Incorrect number of arguments passed to the insert request (it should be 5)";

class InsertNumberException final : public BaseException
{
public:
    InsertNumberException() : BaseException(kNumberArgsError) {}

    [[nodiscard]] const char* name() const noexcept final { return "InsertNumberException"; }
    
    ~InsertNumberException() noexcept final = default;
};