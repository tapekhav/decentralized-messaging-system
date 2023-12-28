#pragma once

#include <string>
#include <exception>

class BaseException : public std::exception
{
public:
    explicit BaseException(std::string msg = "Base exception") : _msg(std::move(msg)) {}

    [[nodiscard]] virtual const char* name() const noexcept { return "BaseException"; } 

    [[nodiscard]] const char* what() const noexcept final { return _msg.c_str(); }

    ~BaseException() noexcept override = default;
private:
    std::string _msg;
};