#pragma once

#include <cstdint>

#include <consts.h>
#include <redis_service.h>

class RunServer
{
public:
    RunServer() :
               _service(redis_db::consts::ip_v4, redis_db::consts::port),
               _server_address(server::consts::ip_v4 + std::string(":") + std::to_string(server::consts::port))
                {}

    void run(); 
private:
    std::string _server_address;
    RedisServiceImpl _service;
};