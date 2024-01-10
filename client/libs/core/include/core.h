#pragma once

#include <service_impl.h>

class RunServer
{
public:
    RunServer(const std::string& ip_v4, uint32_t port);

    void run(); 
private:
    std::string _server_address;
    ClientServiceImpl _service;
};