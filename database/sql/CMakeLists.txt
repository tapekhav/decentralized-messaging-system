cmake_minimum_required(VERSION 3.20)
project(DecentralizedMessagingSystemSQL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include("${CMAKE_CURRENT_BINARY_DIR}/conan_toolchain.cmake")

find_package(gRPC REQUIRED)
find_package(libpqxx REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(Protobuf REQUIRED)

set(PROTO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../protos")
file(GLOB PROTO_FILES "../../protos/*.proto")
include("${CMAKE_CURRENT_SOURCE_DIR}/../../include.cmake")

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/protos
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/core/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/utilities/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/utilities/exceptions/include
    ${gRPC_INCLUDE_DIR}
    ${PROTOBUF_INCLUDE_DIRS}
    ${libpqxx_INCLUDE_DIRS} 
    ${PostgreSQL_INCLUDE_DIRS}
)

add_subdirectory("libs/")

add_executable(
    ${PROJECT_NAME}
    main.cpp
    ${output_files}
)

target_link_libraries(
    ${PROJECT_NAME} 
    utilities
    core
    protobuf::protobuf
    gRPC::grpc++
    gRPC::grpc_plugin_support
    ${libpqxx_LIBRARIES}
    ${PostgreSQL_LIBRARIES}
)