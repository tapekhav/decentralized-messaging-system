#pragma once

#include <base_exception.h>

static const std::string kSizeError = "Incorrect word size under index";

class SizeException final : public BaseException
{
public:
    explicit SizeException() : BaseException(kSizeError) {}

    [[nodiscard]] const char* name() const noexcept final { return "SizeException"; }
    
    ~SizeException() noexcept final = default;
};