set(TARGET_NAME core)

file(GLOB HEADERS include/*.h)
file(GLOB SOURCES src/*.cpp)

add_library(
    ${TARGET_NAME}
    STATIC
    ${HEADERS}
    ${SOURCES}
)

target_include_directories(
    ${TARGET_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../clients/include
)

target_link_libraries(
    ${TARGET_NAME}
    PUBLIC
    clients
    gRPC::grpc++ 
)