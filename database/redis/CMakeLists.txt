cmake_minimum_required(VERSION 3.20)
project(DecentralizedMessagingSystemRedis)

include("${CMAKE_CURRENT_BINARY_DIR}/conan_toolchain.cmake")

find_package(gRPC REQUIRED)
find_package(hiredis REQUIRED)
find_package(Protobuf REQUIRED)

set(PROTO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../protos")
file(GLOB PROTO_FILES "../../protos/*.proto")
include("${CMAKE_CURRENT_SOURCE_DIR}/../../include.cmake")

include_directories(
    ${gRPC_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/core/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/utilities/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/utilities/exceptions/include
    ${PROTOBUF_INCLUDE_DIRS}
    ${HIREDIS_INCLUDE_DIRS}
)

add_subdirectory("libs/")
add_subdirectory("tests/")

add_executable(
    ${PROJECT_NAME}
    main.cpp
    ${output_files}
)

target_link_libraries(
    ${PROJECT_NAME} 
    core
    protobuf::protobuf
    protobuf::libprotoc
    gRPC::grpc++
    gRPC::grpc_plugin_support
    hiredis::hiredis
)