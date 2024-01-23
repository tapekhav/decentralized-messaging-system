cmake_minimum_required(VERSION 3.20)
project(DecentralizedMessagingSystemServer)

include("${CMAKE_CURRENT_BINARY_DIR}/conan_toolchain.cmake")

find_package(nlohmann_json REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(gRPC REQUIRED)
find_package(Protobuf REQUIRED)

set(PROTO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../protos")
file(GLOB PROTO_FILES "../protos/*.proto")
include("${CMAKE_CURRENT_SOURCE_DIR}/../include.cmake")

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${gRPC_INCLUDE_DIR}
    ${PROTOBUF_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${NLOHMANN_JSON_INCLUDE_DIRS}
)

add_subdirectory("libs/")

add_executable(
    ${PROJECT_NAME}
    main.cpp
)

target_link_libraries(
    ${PROJECT_NAME} 
    ${ZeroMQ_LIBRARIES}
    core
    gRPC::grpc++
    gRPC::grpc++_reflection
    protobuf::libprotobuf
    protobuf::protobuf
    protobuf::libprotoc
    Boost::boost
    nlohmann_json::nlohmann_json
)