#pragma once

#include <grpcpp/grpcpp.h>

#include <database.pb.h>

class IpDatabaseServiceImpl final : public server::IpDatabaseService::Service
{
public:
    grpc::Status getIp()
private:

} 


