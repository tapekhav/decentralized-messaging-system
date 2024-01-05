set(TARGET_NAME core)

file(GLOB HEADERS include/*.h)
file(GLOB SOURCES src/*.cpp)

add_library(
    ${TARGET_NAME}
    ${HEADERS}
    ${SOURCES}
)

target_link_libraries(
    ${TARGET_NAME}
    PRIVATE
    utilities
    protobuf::protobuf
    gRPC::grpc
)

target_include_directories(
    ${TARGET_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../utilities/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../utilities/exceptions/include
)

set(SECOND_TARGET_NAME trash_client)

file(GLOB PROTO_H "${CMAKE_CURRENT_SOURCE_DIR}/../../build/protos/*.h")
file(GLOB PROTO_S "${CMAKE_CURRENT_SOURCE_DIR}/../../build/protos/*.cc")

add_executable(
    ${SECOND_TARGET_NAME}
    trash_client/main.cpp
    ${PROTO_H}
    ${PROTO_S}
 )

target_link_libraries(
    ${SECOND_TARGET_NAME}
    PRIVATE
    ${TARGET_NAME}
    utilities
    protobuf::protobuf
    gRPC::grpc
    gRPC::grpc++
    gRPC::grpc++_unsecure
    protobuf::libprotobuf
)

target_include_directories(
    ${SECOND_TARGET_NAME}
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/protos
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../utilities/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../utilities/exceptions/include
)
