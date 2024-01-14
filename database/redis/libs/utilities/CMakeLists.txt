set(TARGET_NAME utilities)

file(GLOB HEADERS include/*.h)
file(GLOB EXCEPTIONS_HEADERS exceptions/include/*.h)

add_library(
    ${TARGET_NAME}
    INTERFACE
    ${HEADERS}
    ${EXCEPTIONS_HEADERS}
)

target_include_directories(
    ${TARGET_NAME}
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/exceptions/include
)