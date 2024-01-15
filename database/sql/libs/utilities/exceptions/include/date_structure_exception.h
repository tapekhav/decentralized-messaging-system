#pragma once

#include <base_exception.h>

static const std::string kDateStructureError = "Incorrect structure of date(it should be YYYY-MM-DD)";

class DateStructureException final : public BaseException
{
public:
    DateStructureException() : BaseException(kDateStructureError) {}
    DateStructureException(const DateStructureException& other) = delete;
    DateStructureException(DateStructureException&& other) = delete;

    auto operator=(const DateStructureException& other) -> DateStructureException& = delete;
    auto operator=(DateStructureException&& other) -> DateStructureException& = delete;

    [[nodiscard]] auto name() const noexcept -> const char* final { return "DateStructureException"; }
    
    ~DateStructureException() noexcept final = default;
};