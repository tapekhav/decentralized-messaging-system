set(TARGET_NAME utilities)

file(GLOB HEADERS include/*.h)
file(GLOB EXCEPTIONS_HEADERS exceptions/include/*.h)

add_library(
    ${TARGET_NAME}
    ${EXCEPTIONS_HEADERS}
    ${HEADERS}
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/exceptions/include
    ${SOCI_INCLUDE_DIRS}
)