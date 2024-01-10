#include <service_impl.h>

#include <zmq.hpp>

ClientServiceImpl::ClientServiceImpl(std::string ip_v4, uint32_t port) 
                                     : 
                                     _ip_sender(std::move(ip_v4)), 
                                     _port_sender(port) {}

Status ClientServiceImpl::sendMessage(ServerContext* context, 
                                      const clientMessage* request, 
                                      Empty* response)
{
    try
    {
        zmq::context_t zmq_context(1);
        zmq::socket_t zmq_socket(zmq_context, ZMQ_PAIR);

        std::string server_address = "tcp://" + request->ip_reciever() + 
                                    ":" + std::to_string(request->port_reciever());
        zmq_socket.connect(server_address);

        zmq::message_t msg(request->message().c_str(), request->message().size());
        zmq_socket.send(msg, zmq::send_flags::dontwait);

        return Status::OK; 
    }
    catch (const zmq::error_t& e) 
    {
        std::cerr << "ZeroMQ error: " << e.what() << '\n';
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Exception: " << e.what() << '\n';
    } 
    catch (...) 
    {
        std::cerr << "Unknown exception occurred.\n";
    }

    return Status(grpc::StatusCode::INTERNAL, "Error sending message");
}