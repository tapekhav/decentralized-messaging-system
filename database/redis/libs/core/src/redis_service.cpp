#include <redis_service.h>

Status RedisServiceImpl::getIp(ServerContext* context, 
                               const IpRequest* request, 
                               IpResponse* response)
{
    response->set_ip_v4(_cache_manager->readValue(request->nickname()));

    return Status::OK;
}

Status RedisServiceImpl::setUser(ServerContext* context, 
                                 const UserRequest* request, 
                                 Empty* response)
{
    _cache_manager->createValue({request->nickname(), request->ip_v4()});

    return Status::OK;
} 