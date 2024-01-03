#pragma once

#include <grpcpp/grpcpp.h>

#include <database.grpc.pb.h>

class IpDatabaseServiceImpl final : public server::IpDatabaseService::Service
{
public:
    auto getIp(grpc::ServerContext* context, 
               const server::IpRequest* request,
               server::DatabaseResponse* response) -> grpc::Status final;
};