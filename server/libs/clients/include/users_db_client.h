#pragma once

#include <string>
#include <memory>

#include <grpcpp/grpcpp.h>
#include <db_sql.grpc.pb.h>

using grpc::Status;
using grpc::ClientContext;
using google::protobuf::Empty;

class UsersClient
{
public:
    explicit UsersClient(std::shared_ptr<grpc::Channel> channel)
                         : 
                         _stub(db_sql::SqlDatabaseService::NewStub(channel)) {}

    auto getUserIp(const std::string& nickname) -> std::string;

    auto setUser(
        const std::string& nickname,
        const std::string& ip,
        const std::string& birth_date,
        const std::string& name,
        const std::string& additional_information
    ) -> Status;
private:
    std::unique_ptr<db_sql::SqlDatabaseService::Stub> _stub;
};