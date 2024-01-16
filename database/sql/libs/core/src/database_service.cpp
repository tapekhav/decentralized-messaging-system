#include <database_service.h>

Status SqlDatabaseServiceImpl::getUser(ServerContext* context, 
                                       const UserRequest* request,
                                       UserResponse* response)
{
    auto user = _db_manager.selectUserByNickname(request->nickname());

    if (user.empty())
    {
        return Status(grpc::StatusCode::NOT_FOUND, "User not found");
    }

    response->set_user_id(std::stoi(user[0]));
    response->set_ip(user[2]);
    response->set_birth_date(user[3]);
    response->set_name(user[4]);
    response->set_additional_information(user[5]);

    return Status::OK;
}

Status SqlDatabaseServiceImpl::setUser(ServerContext* context, 
                                       const NewUserRequest* request,
                                       Empty* response)
{
    _db_manager.insertQuery({
        request->nickname(), 
        request->ip(), 
        request->name(), 
        request->birth_date(), 
        request->additional_information()
    });

    return Status::OK;
}