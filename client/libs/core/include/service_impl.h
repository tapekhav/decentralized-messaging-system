#pragma once

#include <cstdint>

#include <client.grpc.pb.h>

using grpc::Status;
using grpc::ServerContext;
using client::clientMessage;
using google::protobuf::Empty;

class ClientServiceImpl final : public client::ClientService::Service
{
public:
    ClientServiceImpl(std::string ip_v4, uint32_t port);
    Status sendMessage(ServerContext* context, 
                       const clientMessage* request, 
                       Empty* response) final;
private:
    std::string _ip_sender;
    uint32_t _port_sender;
};