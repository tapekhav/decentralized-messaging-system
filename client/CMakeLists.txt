cmake_minimum_required(VERSION 3.20)
project(DecentralizedMessagingClient)

include("${CMAKE_CURRENT_SOURCE_DIR}/../include.cmake")

find_package(ZeroMQ REQUIRED)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${gRPC_INCLUDE_DIR}
    ${PROTOBUF_INCLUDE_DIRS}
    ${ZeroMQ_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_subdirectory("libs/")

add_executable(
    ${PROJECT_NAME}
    main.cpp
)


target_link_libraries(
    ${PROJECT_NAME} 
    core
    protobuf::protobuf
    gRPC::grpc++
    gRPC::grpc_plugin_support
    libzmq-static
)

