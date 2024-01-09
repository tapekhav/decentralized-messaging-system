#include <server.h>

auto Server::createChannel(const std::string& ip_v4, uint32_t port) -> std::shared_ptr<grpc::Channel>
{
    return grpc::CreateChannel(
        ip_v4 + std::string(":") + std::to_string(port), 
        grpc::InsecureChannelCredentials()
    );
}