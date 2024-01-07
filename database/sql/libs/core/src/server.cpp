#include <server.h>

#include <iostream>


RunServer::RunServer(const std::string& ip_v4, std::size_t port) 
               : _uri(ip_v4 + std::string(":") + std::to_string(port)) {}

void RunServer::runDatabaseService()
{
    grpc::ServerBuilder builder;
    builder.AddListeningPort(_uri, grpc::InsecureServerCredentials());

    IpDatabaseServiceImpl service;
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << _uri << '\n';

    server->Wait();
}