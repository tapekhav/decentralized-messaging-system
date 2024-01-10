#include <core.h>

#include <grpcpp/grpcpp.h>

#include <consts.h>

RunServer::RunServer(const std::string& ip_v4, uint32_t port) 
                     :
                     _service(client_service::consts::ip_v4, client_service::consts::port),
                     _server_address(ip_v4 + std::string(":") + std::to_string(port)) {}

void RunServer::run()
{
    grpc::ServerBuilder builder;
    builder.AddListeningPort(_server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&_service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << _server_address << '\n';

    server->Wait();
}