#pragma once

#include <memory>

#include <grpcpp/grpcpp.h>
#include <database_redis.grpc.pb.h>

#include <cache_manager.h>

using CacheType = CacheManager<int32_t, std::string>;

class RedisServiceImpl final : public redis::RedisService::Service
{
public:
    RedisServiceImpl(const std::string& ip_v4, int32_t port) 
                     : _cache_manager(std::make_unique<CacheType>(ip_v4, port)) {}

    grpc::Status getIp(grpc::ServerContext* context, 
                       const redis::IpRequest* request, 
                       redis::IpResponse* response) final;

    grpc::Status setUser(grpc::ServerContext* context, 
                         const redis::UserRequest* request, 
                         google::protobuf::Empty* response) final; 
private:
    std::unique_ptr<CacheType> _cache_manager;
};
