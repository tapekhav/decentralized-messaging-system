#include <core.h>


void RunServer::run()
{
    grpc::ServerBuilder builder;
    builder.AddListeningPort(_server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&_service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << _server_address << std::endl;
    server->Wait();
}