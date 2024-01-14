#pragma once

#include <redis_base_error.h>

static const std::string kMessage = "Context is NULL";

class ContextNullptrError final : public RedisBaseError
{
public:
    ContextNullptrError() noexcept : RedisBaseError(kMessage) {};

    ContextNullptrError(const ContextNullptrError&) = delete;
    ContextNullptrError(ContextNullptrError&&) = delete;

    auto operator=(const ContextNullptrError&) -> ContextNullptrError& = delete;
    auto operator=(ContextNullptrError&&) -> ContextNullptrError& = delete;

    [[nodiscard]] auto name() const noexcept -> const char* final { return "ContextNullptrError"; }

    ~ContextNullptrError() noexcept final = default;
};