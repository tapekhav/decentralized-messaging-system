#include <redis_service.h>

grpc::Status RedisServiceImpl::getIp(grpc::ServerContext* context, 
                                     const redis::IpRequest* request, 
                                     redis::IpResponse* response)
{
    response->set_ip_v4(_cache_manager->readValue(request->user_id()));

    return grpc::Status::OK;
}

grpc::Status RedisServiceImpl::setUser(grpc::ServerContext* context, 
                                       const redis::UserRequest* request, 
                                       google::protobuf::Empty* response)
{
    _cache_manager->createValue({request->user_id(), request->ip_v4()});

    return grpc::Status::OK;
} 