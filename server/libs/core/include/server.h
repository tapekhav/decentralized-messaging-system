#pragma once

#include <optional>

#include <grpcpp/grpcpp.h>
#include <db_sql.pb.h>

#include <cache_client.h>
#include <users_db_client.h>

using grpc::Status;
using grpc::Channel;

class Server 
{
public:
    Server() { createChannels(); }
    //! Get from redis
    auto getUserIp(const std::string& nickname) -> std::string;

    ~Server() = default;

    auto createSession(const std::string& nickname, const std::string& ip_v4) -> Status;
    auto createChannel(const std::string& ip_v4, uint32_t port) -> std::shared_ptr<Channel>;

    void createChannels();
private:
    std::unique_ptr<UsersClient> _users_client;
    std::unique_ptr<CacheClient> _cache_client;
};