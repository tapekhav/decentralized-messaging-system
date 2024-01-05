#pragma once 

#include <grpcpp/grpcpp.h>
#include <db_sql.grpc.pb.h>

#include <users_db_manager.h>

class IpDatabaseServiceImpl final : public db_sql::IpDatabaseService::Service
{
public:
    IpDatabaseServiceImpl() : _db_manager(UsersDatabaseManager::getInstance()) {}

    grpc::Status getIp(grpc::ServerContext* context, 
                       const db_sql::IpRequest* request,
                       db_sql::IpResponse* response) final;
private:
    UsersDatabaseManager& _db_manager;
};