#pragma once

#include <grpcpp/grpcpp.h>

#include <db_sql.grpc.pb.h>

class SqlDatabaseServiceClient 
{
public:
    explicit SqlDatabaseServiceClient(std::shared_ptr<grpc::Channel> channel)
                        : _stub(db_sql::SqlDatabaseService::NewStub(channel)) {}

    std::string getUser(const std::string& nickname);
private:
    std::unique_ptr<db_sql::SqlDatabaseService::Stub> _stub;
};