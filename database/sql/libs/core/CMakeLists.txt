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
)

target_include_directories(
    ${TARGET_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../utilities/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../utilities/exceptions/include
)