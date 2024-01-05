#include "../include/server.h"

#include <iostream>

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