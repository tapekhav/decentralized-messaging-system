#pragma once

#include <optional>

#include <cache_client.h>
#include <users_db_client.h>

class Server 
{
public:
    //! Get from redis
    std::optional<std::string> getIpFromSession(uint32_t user_id) { return std::nullopt; }
    //! Save in Redis data
    void createSession(uint32_t user_id, const std::string& ip_v4) { }
    auto createChannel(const std::string& ip_v4, uint32_t port) -> std::shared_ptr<grpc::Channel>;
    void createChannels() {}
private:
    std::unique_ptr<UsersClient> _users_client;
    std::unique_ptr<CacheClient> _cache_client;
};