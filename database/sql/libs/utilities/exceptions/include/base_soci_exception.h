#pragma once

#include <soci/error.h>

class SociBaseException : public soci::soci_error
{
public:
    explicit SociBaseException(const std::string& msg) : soci::soci_error(msg) {}

    [[nodiscard]] virtual const char* name() const noexcept { return "SociBaseException"; } 

    [[nodiscard]] const char* what() const noexcept override { return get_error_message().c_str(); }

    ~SociBaseException() noexcept override = default;
};