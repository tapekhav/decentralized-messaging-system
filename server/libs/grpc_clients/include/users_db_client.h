#pragma once

#include <memory>

#include <grpcpp/grpcpp.h>

class UsersClient
{
public:
    UsersClient(const std::shared_ptr<grpc::Channel>& channel) {}
    void getUser(const std::string& nickname);
private:
    //! TODO: write stub for sql db service
};