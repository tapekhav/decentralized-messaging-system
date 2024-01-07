#include <client.h>

std::string IpDatabaseServiceClient::getIp(const std::string& nickname)
{
    db_sql::IpRequest request;
    request.set_nickname(nickname);

    db_sql::IpResponse response;

    grpc::ClientContext context;
    grpc::Status status = _stub->getIp(&context, request, &response);

    return status.ok() ? response.ip() : status.error_message();
}