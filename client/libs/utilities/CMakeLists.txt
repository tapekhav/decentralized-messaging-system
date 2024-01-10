set(TARGET_NAME utilities)

file(GLOB HEADERS include/*.h)

add_library(
    ${TARGET_NAME}
    INTERFACE
    ${HEADERS}
)

target_include_directories(
    ${TARGET_NAME}
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)