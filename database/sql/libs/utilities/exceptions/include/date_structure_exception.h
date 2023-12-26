#pragma once

#include <base_soci_exception.h>

static const std::string kDateStructureError = "Incorrect structure of date(it should be YYYY-MM-DD)";

class DateStructureException final : public SociBaseException
{
public:
    DateStructureException() : SociBaseException(kDateStructureError) {}

    [[nodiscard]] const char* name() const noexcept final { return "DateStructureException"; }
    
    ~DateStructureException() noexcept final = default;
};