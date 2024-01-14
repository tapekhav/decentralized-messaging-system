#include <client.h>

std::string SqlDatabaseServiceClient::getUser(const std::string& nickname)
{
    db_sql::UserRequest request;
    request.set_nickname(nickname);

    db_sql::UserResponse response;

    grpc::ClientContext context;
    grpc::Status status = _stub->getUser(&context, request, &response);

    return status.ok() ? response.ip() : status.error_message();
}