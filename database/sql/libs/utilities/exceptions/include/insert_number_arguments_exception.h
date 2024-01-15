#pragma once

#include <base_exception.h>

static const std::string kNumberArgsError = "Incorrect number of arguments passed to the insert request (it should be 5)";

class InsertNumberException final : public BaseException
{
public:
    InsertNumberException() : BaseException(kNumberArgsError) {}
    InsertNumberException(const InsertNumberException& other) = delete;
    InsertNumberException(InsertNumberException&& other) = delete;

    auto operator=(const InsertNumberException& other) -> InsertNumberException& = delete;
    auto operator=(InsertNumberException&& other) -> InsertNumberException& = delete;

    [[nodiscard]] auto name() const noexcept -> const char* final
                                                         { return "InsertNumberException"; }
    
    ~InsertNumberException() noexcept final = default;
};