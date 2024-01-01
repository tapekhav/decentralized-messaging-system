#pragma once 

#include <string>
#include <exception>

class RedisBaseError : public std::exception
{
public:
    explicit RedisBaseError(std::string msg) noexcept : _msg(std::move(msg)) {};

    RedisBaseError(const RedisBaseError&) = delete;
    RedisBaseError(RedisBaseError&&) = delete;

    auto operator=(const RedisBaseError&) -> RedisBaseError& = delete;
    auto operator=(RedisBaseError&&) -> RedisBaseError& = delete;

    [[nodiscard]] virtual auto name() const noexcept -> const char* { return "RedisBaseError"; }
    [[nodiscard]] auto what() const noexcept -> const char* final { return _msg.c_str(); }

    ~RedisBaseError() noexcept override = default;
private:
    std::string _msg;
};