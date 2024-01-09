#pragma once

#include <memory>

#include <grpcpp/grpcpp.h>

class CacheClient
{
public:
    CacheClient(const std::shared_ptr<grpc::Channel>& channel) {}
    void getIp(uint32_t user_id);
private:
    //! TODO: write stub for redis db service
};