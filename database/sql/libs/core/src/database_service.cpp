#include <database_service.h>

grpc::Status IpDatabaseServiceImpl::getIp(grpc::ServerContext* context, 
                                          const db_sql::IpRequest* request,
                                          db_sql::IpResponse* response)
{
    auto user = _db_manager.selectUserByNickname(request->nickname());

    if (user.empty())
    {
        return grpc::Status(grpc::StatusCode::NOT_FOUND, "User not found");
    }

    response->set_user_id(std::stoi(user[0]));
    response->set_ip(user[2]);
    response->set_birth_date(user[3]);
    response->set_name(user[4]);
    response->set_additional_information(user[5]);

    return grpc::Status::OK;
}