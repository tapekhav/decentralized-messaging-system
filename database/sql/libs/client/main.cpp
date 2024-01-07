#include <memory>

#include <client.h>

int main()
{
    std::string server_address("127.0.0.1:50051");

    auto channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
    IpDatabaseServiceClient client(channel);

    std::string nickname = "test4";
    std::string ip = client.getIp(nickname);

    if (!ip.empty())
    {
        std::cout << "IP for user " << nickname << ": " << ip << std::endl;
    }
    else
    {
        std::cerr << "Failed to retrieve IP for user " << nickname << std::endl;
    }

    return 0;
}