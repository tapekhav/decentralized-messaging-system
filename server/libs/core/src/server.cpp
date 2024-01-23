#include <server.h>

auto Server::createChannel(const std::string& ip_v4, uint32_t port) -> std::shared_ptr<grpc::Channel>
{
    return grpc::CreateChannel(
        ip_v4 + std::string(":") + std::to_string(port), 
        grpc::InsecureChannelCredentials()
    );
}

auto Server::createSession(const std::string& nickname, const std::string& ip_v4) -> Status
{
    return _cache_client->setUser(nickname, ip_v4);
}

void Server::createChannels()
{
    _users_client = std::make_unique<UsersClient>();
    _cache_client = std::make_unique<CacheClient>(createChannel("127.0.0.1", 50052));
}

auto Server::getUserIp(const std::string& nickname) -> std::string
{
    auto response = _cache_client->getUser(nickname);

    if (response.has_value())
    {
        return response.value();
    }

    auto ip = _users_client->getUserIp(nickname);
    createSession(nickname, ip);

    return ip;
}