#pragma once 

#include <cstddef>
#include <string>

#include <database_service.h>

class RunServer
{
public:
    RunServer(const std::string& ip_v4, std::size_t port);

    void runDatabaseService();

private:    
    std::string _uri;
};
