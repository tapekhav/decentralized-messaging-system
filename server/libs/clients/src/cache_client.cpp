#include <cache_client.h>

auto CacheClient::getUser(const std::string& nickname) -> std::optional<std::string>
{
    redis::IpRequest request;
    request.set_nickname(nickname);

    redis::IpResponse response;

    ClientContext context;
    Status status = _stub->getIp(&context, request, &response);

    if (status.ok())
    {
        return response.ip_v4();
    }
    return std::nullopt;
}

auto CacheClient::setUser(const std::string& nickname, const std::string& ip) -> Status
{
    redis::UserRequest request;
    request.set_nickname(nickname);
    request.set_ip_v4(ip);

    Empty response;
    ClientContext context;

    return _stub->setUser(&context, request, &response);
}