#pragma once

#include <ip_database_manager.h>

enum Services
{

};

class GrpcServer
{
public:
    explicit GrpcServer(std::string  server_address) : _server_address(std::move(server_address)) {}

    void runUserService();

private:
    template <typename ServiceType>
    void runServer(ServiceType& service, const std::string& service_name);

private:
    std::string _server_address;
};

template <typename ServiceType>
void GrpcServer::runServer(ServiceType& service, const std::string& service_name)
{
    grpc::ServerBuilder builder;
    builder.AddListeningPort(_server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << service_name << " listening on " << _server_address << std::endl;
    server->Wait();
}

void GrpcServer::runUserService()
{
    IpDatabaseServiceImpl service;
    RunServer(service, "User service");
}