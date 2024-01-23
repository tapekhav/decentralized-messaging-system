#pragma once

#include <memory>
#include <optional>

#include <grpcpp/grpcpp.h>
#include <database_redis.grpc.pb.h>

using grpc::Status;
using grpc::ClientContext;
using google::protobuf::Empty;

class CacheClient
{
public:
    explicit CacheClient(const std::shared_ptr<grpc::Channel>& channel) 
                         :
                         _stub(redis::RedisService::NewStub(channel)) {}
                    
    auto getUser(const std::string& nickname) -> std::optional<std::string>;
    auto setUser(const std::string& nickname, const std::string& ip) -> Status;
private:
    std::unique_ptr<redis::RedisService::Stub> _stub;
};