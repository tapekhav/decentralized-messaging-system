#include <memory>

#include <client.h>

int main()
{
    std::string server_address("127.0.0.1:50051");

    auto channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
    SqlDatabaseServiceClient client(channel);

    std::string nickname = "test4";
    std::string ip = client.getUser(nickname);

    if (!ip.empty())
    {
        std::cout << "IP for user " << nickname << ": " << ip << '\n';
    }
    else
    {
        std::cerr << "Failed to retrieve IP for user " << nickname << '\n';
    }

    return 0;
}