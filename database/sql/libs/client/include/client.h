#pragma once

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