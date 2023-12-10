set(TARGET_NAME ip_db_connector)

file(GLOB HEADERS include/*.h)
file(GLOB SOURCES src/*.cpp)

add_library(${TARGET_NAME} ${SOURCES})

target_include_directories(
    ${TARGET_NAME}
    PUBLIC
    ${GRPC_INCLUDE_DIRS}
    ${gRPC_INCLUDE_DIR}
    ${PROTOBUF_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${ABSEIL_INCLUDE_DIRS}
    ${CONAN_INCLUDE_DIRS}
)

target_sources(
    ${TARGET_NAME}
    PUBLIC
    ${HEADERS} 
    ${SOURCES}
)