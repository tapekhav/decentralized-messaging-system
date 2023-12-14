#pragma once

#include <grpcpp/grpcpp.h>
#include <grpcpp/support/status.h>
#include <grpcpp/server_context.h>

#include <database.grpc.pb.h>

class IpDatabaseServiceImpl final : public server::IpDatabaseService::Service
{
public: 
    grpc::Status getIp(grpc::ServerContext* context, 
                       const server::IpRequest* request,
                       server::IpResponse* response) final;

private:
};