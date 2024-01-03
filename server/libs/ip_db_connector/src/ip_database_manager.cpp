#include <ip_database_manager.h>
#include <memory>

auto IpDatabaseServiceImpl::getIp(grpc::ServerContext* context, 
                                  const server::IpRequest* request,
                                  server::DatabaseResponse* response) -> grpc::Status
{
    
}