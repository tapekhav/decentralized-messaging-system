#pragma once 

#include <grpcpp/grpcpp.h>
#include <db_sql.grpc.pb.h>

#include <users_db_manager.h>

using grpc::Status;
using grpc::ServerContext;
using db_sql::UserRequest;
using db_sql::UserResponse;
using db_sql::NewUserRequest;
using google::protobuf::Empty;

class SqlDatabaseServiceImpl final : public db_sql::SqlDatabaseService::Service
{
public:
    SqlDatabaseServiceImpl() : _db_manager(UsersDatabaseManager::getInstance()) {}

    Status getUser(ServerContext* context, 
                   const UserRequest* request,
                   UserResponse* response) final;

    Status setUser(ServerContext* context, 
                   const NewUserRequest* request,
                   Empty* response) final;
private:
    UsersDatabaseManager& _db_manager;
};