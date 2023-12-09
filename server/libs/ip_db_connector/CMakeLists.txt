set(TARGET_NAME ip_db_connector)

find_package(gRPC REQUIRED)

add_library(${TARGET_NAME})

file(GLOB HEADERS include/*.h)
file(GLOB SOURCES src/*.cpp)

include_directories(
    include
    ${GRPC_INCLUDE_DIRS}
)

target_sources(
    ${TARGET_NAME}
    PUBLIC
    ${HEADERS} 
    ${SOURCES}
)