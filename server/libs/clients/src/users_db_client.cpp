#include <users_db_client.h>

#include <nlohmann/json.hpp>

using boost::beast::http::verb::get;
using boost::beast::http::verb::post;
using boost::beast::http::verb::put;
using boost::beast::http::verb::delete_;

using boost::beast::http::field::user_agent;
using boost::beast::http::field::content_type;

using boost::beast::flat_buffer;

UsersClient::UsersClient(const std::string& link, const std::string& port)
        :
        _resolver(_io_context),
        _stream(_io_context),
        _user_agent("Beast"),
        _content_type("application/json")
{
    auto results = _resolver.resolve(link, port);
    _stream.connect(results.begin()->endpoint());
}

auto UsersClient::performRequest(request& req) -> responce
{
    req.set(user_agent, _user_agent);
    req.set(content_type, _content_type);

    boost::beast::http::write(_stream, req);

    flat_buffer buffer;
    responce res;
    boost::beast::http::read(_stream, buffer, res);

    return res;
}

auto UsersClient::getUserIp(const std::string& nickname) -> std::string
{
    request req(get, "/users/" + nickname, 20);
    auto res = performRequest(req);

    auto json_obj = nlohmann::json::parse(res.body());

    return json_obj["ipv4"].dump();
}

auto UsersClient::setUser(
        const std::string& nickname,
        const std::string& ip,
        const std::string& birth_date,
        const std::string& name,
        const std::string& additional_information
    ) -> status
{
    nlohmann::json data =
    {
        {"nickname", nickname},
        {"ipv4", ip},
        {"name", name},
        {"birthDate", birth_date},
        {"additionalInformation", additional_information}
    };

    std::string request_body = data.dump();
    request req(post, "/setUser", 20);
    req.set(content_type, "application/json");
    req.body() = request_body;
    req.prepare_payload();

    auto res = performRequest(req);

    return res.result();
}