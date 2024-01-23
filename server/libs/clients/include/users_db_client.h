#pragma once

#include <string>
#include <memory>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

using request = boost::beast::http::request<boost::beast::http::string_body>;
using responce = boost::beast::http::response<boost::beast::http::string_body>;

using boost::beast::http::status;

class UsersClient
{
public:
    explicit UsersClient(
            const std::string& link = "localhost",
            const std::string& port = "8080"
    );
    auto getUserIp(const std::string& nickname) -> std::string;
    auto setUser(
            const std::string& nickname,
            const std::string& ip,
            const std::string& birth_date,
            const std::string& name,
            const std::string& additional_information
    ) -> status;

private:
    auto performRequest(request& req) -> responce;

    boost::asio::io_context _io_context;
    boost::asio::ip::tcp::resolver _resolver;
    boost::beast::tcp_stream _stream;

    std::string _user_agent;
    std::string _content_type;
};