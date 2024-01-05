#include <memory>

#include <grpcpp/grpcpp.h>
#include <db_sql.grpc.pb.h>

class IpDatabaseServiceClient 
{
public:
    explicit IpDatabaseServiceClient(std::shared_ptr<grpc::Channel> channel)
                        : _stub(db_sql::IpDatabaseService::NewStub(channel)) {}

    std::string getIp(const std::string& nickname);
private:
    std::unique_ptr<db_sql::IpDatabaseService::Stub> _stub;
};

std::string IpDatabaseServiceClient::getIp(const std::string& nickname)
{
    db_sql::IpRequest request;
    request.set_nickname(nickname);

    db_sql::IpResponse response;

    grpc::ClientContext context;
    grpc::Status status = _stub->getIp(&context, request, &response);

    return status.ok() ? response.ip() : status.error_message();
}

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