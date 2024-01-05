#pragma once

#include <typeinfo>
#include <functional>

#include <ip_database_manager.h>

struct TypeInfoPtrComparator
{
    auto operator()(const std::type_info* lhs, const std::type_info* rhs) const -> bool 
    {
        return lhs->before(*rhs);
    }
};

const static std::map<const std::type_info*, std::string, TypeInfoPtrComparator> kServiceIdMap = 
{
    { &typeid(IpDatabaseServiceImpl), "Ip Database Service" }
}; 

template <typename ServiceType>
class GrpcServer
{
public:
    explicit GrpcServer(std::string server_address) : _server_address(std::move(server_address)) {}

    void runService();

private:
    void runServer(ServiceType& service, const std::string& service_name);

private:
    std::string _server_address;
};

template <typename ServiceType>
void GrpcServer<ServiceType>::runServer(ServiceType& service, const std::string& service_name)
{
    grpc::ServerBuilder builder;
    builder.AddListeningPort(_server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << service_name << " listening on " << _server_address << std::endl;
    server->Wait();
}

template <typename ServiceType>
void GrpcServer<ServiceType>::runService()
{
    ServiceType service;
    runServer(service, kServiceIdMap.at(&typeid(ServiceType)));
}