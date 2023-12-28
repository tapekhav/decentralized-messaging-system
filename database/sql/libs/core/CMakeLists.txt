set(TARGET_NAME core)

file(GLOB HEADERS include/*.h)
file(GLOB SOURCES src/*.cpp)

add_library(
    ${TARGET_NAME}
    ${HEADERS}
    ${SOURCES}
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
    ${TARGET_NAME}
    PRIVATE
    utilities
)