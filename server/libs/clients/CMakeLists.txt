set(TARGET_NAME clients)

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
)

target_link_libraries(
    ${TARGET_NAME}
    PUBLIC
    gRPC::grpc++ 
)