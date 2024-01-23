#pragma once

#include <memory>

#include <grpcpp/grpcpp.h>
#include <database_redis.grpc.pb.h>

#include <cache_manager.h>

using grpc::Status;
using redis::IpRequest;
using redis::IpResponse;
using redis::UserRequest;
using grpc::ServerContext;
using google::protobuf::Empty;
using CacheType = CacheManager<int32_t, std::string>;

class RedisServiceImpl final : public redis::RedisService::Service
{
public:
    RedisServiceImpl(const std::string& ip_v4, int32_t port) 
                     : _cache_manager(std::make_unique<CacheType>(ip_v4, port)) {}

    Status getIp(ServerContext* context, 
                       const IpRequest* request, 
                       IpResponse* response) final;

    Status setUser(ServerContext* context, 
                         const UserRequest* request, 
                         Empty* response) final; 
private:
    std::unique_ptr<CacheType> _cache_manager;
};
