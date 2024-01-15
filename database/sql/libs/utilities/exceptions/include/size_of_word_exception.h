#pragma once

#include <base_exception.h>

static const std::string kSizeError = "Incorrect word size under index";

class SizeException final : public BaseException
{
public:
    explicit SizeException() : BaseException(kSizeError) {}
    SizeException(const SizeException& other) = delete;
    SizeException(SizeException&& other) = delete;

    auto operator=(const SizeException& other) -> SizeException& = delete;
    auto operator=(SizeException&& other) -> SizeException& = delete;

    [[nodiscard]] auto name() const noexcept -> const char* final { return "SizeException"; }
    
    ~SizeException() noexcept final = default;
};