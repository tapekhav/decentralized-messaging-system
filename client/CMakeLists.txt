cmake_minimum_required(VERSION 3.20)
project(DecentralizedMessagingClient)

include("${CMAKE_CURRENT_SOURCE_DIR}/../include.cmake")

find_package(ZeroMQ REQUIRED)
find_package(gRPC REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/core/include
    ${CMAKE_CURRENT_BINARY_DIR}
    ${gRPC_INCLUDE_DIR}
    ${PROTOBUF_INCLUDE_DIRS}
    ${ZeroMQ_INCLUDE_DIRS}
)

add_subdirectory("libs/")

add_executable(
    ${PROJECT_NAME}
    main.cc
    ${output_files}
)

target_link_libraries(
    ${PROJECT_NAME} 
    core
    protobuf::protobuf
    gRPC::grpc++
    gRPC::grpc_plugin_support
    libzmq-static
)

