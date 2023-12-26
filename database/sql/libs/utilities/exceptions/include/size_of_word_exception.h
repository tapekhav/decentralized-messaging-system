#pragma once

#include <base_soci_exception.h>
#include <string>

static const std::string kSizeError = "Incorrect word size under index: ";

class SizeException final : public SociBaseException
{
public:
    explicit SizeException(std::size_t number) : SociBaseException(kSizeError + std::to_string(number)) {}

    [[nodiscard]] const char* name() const noexcept final { return "SizeException"; }
    
    ~SizeException() noexcept final = default;
};