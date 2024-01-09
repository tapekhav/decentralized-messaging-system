#pragma once

#include <vector>

#include <cache_client.h>
#include <client_session.h>
#include <users_db_client.h>

class Core 
{
public: 
    //! Get from redis
    std::string getIpFromSession(uint32_t user_id) {}
    //! Save in Redis data
    void createSession(uint32_t user_id, const std::string& ip_v4) { }
    auto createChannel(const std::string& ip_v4, uint32_t port) -> std::shared_ptr<grpc::Channel>;
    void createChannels() {}
private:
    std::unique_ptr<UsersClient> _users_client;
    std::unique_ptr<CacheClient> _cache_client;
};
