cmake_minimum_required(VERSION 3.20)
project(DecentralizedMessagingClient)

include("${CMAKE_CURRENT_BINARY_DIR}/conan_toolchain.cmake")

find_package(gRPC REQUIRED)
find_package(Protobuf REQUIRED)
find_package(ZeroMQ REQUIRED)

set(PROTO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../protos")
file(GLOB PROTO_FILES "../protos/*.proto")
set(PROJ_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")
include("${CMAKE_CURRENT_SOURCE_DIR}/../include.cmake")

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

