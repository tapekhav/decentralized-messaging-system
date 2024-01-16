#pragma once

#include <redis_base_error.h>

class ContextError final : public RedisBaseError
{
public:
    explicit ContextError(const std::string& msg) noexcept : RedisBaseError(msg) {}

    ContextError(const ContextError&) = delete;
    ContextError(ContextError&&) = delete;

    auto operator=(const ContextError&) -> ContextError& = delete;
    auto operator=(ContextError&&) -> ContextError& = delete;

    [[nodiscard]] auto name() const noexcept -> const char* final { return "ContextError"; }

    ~ContextError() noexcept final = default;
};