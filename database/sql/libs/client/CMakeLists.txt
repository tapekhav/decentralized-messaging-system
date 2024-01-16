set(TARGET_NAME client)

file(GLOB PROTO_H "${CMAKE_CURRENT_SOURCE_DIR}/../../build/*.h")
file(GLOB PROTO_S "${CMAKE_CURRENT_SOURCE_DIR}/../../build/*.cc")

file(GLOB CLIENT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp") 

add_executable(
    ${TARGET_NAME}
    main.cpp
    ${PROTO_H}
    ${PROTO_S}
    ${CLIENT_SOURCE}
)

target_link_libraries(
    ${TARGET_NAME} 
    PRIVATE
    core
    utilities
    protobuf::protobuf
    gRPC::grpc++
    gRPC::grpc_plugin_support
)

target_include_directories(
    ${TARGET_NAME}
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/../../
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../utilities/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../utilities/exceptions/include
)

