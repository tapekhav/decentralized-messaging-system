#pragma once

#include <string>
#include <exception>

class BaseException : public std::exception
{
public:
    explicit BaseException(std::string msg = "Base exception") : _msg(std::move(msg)) {}
    BaseException(const BaseException& other) = delete;
    BaseException(BaseException&& other) = delete;

    auto operator=(const BaseException& other) -> BaseException& = delete;
    auto operator=(BaseException&& other) -> BaseException& = delete;

    [[nodiscard]] virtual auto name() const noexcept -> const char* { return "BaseException"; } 

    [[nodiscard]] auto what() const noexcept -> const char* final { return _msg.c_str(); }

    ~BaseException() noexcept override = default;
private:
    std::string _msg;
};