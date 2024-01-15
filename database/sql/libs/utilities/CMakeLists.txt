set(TARGET_NAME utilities)

file(GLOB HEADERS include/*.h)
file(GLOB EXCEPTIONS_HEADERS exceptions/include/*.h)
file(GLOB SOURCE src/*.cpp)

add_library(
    ${TARGET_NAME}
    ${EXCEPTIONS_HEADERS}
    ${HEADERS}
    ${SOURCE}
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/exceptions/include
)

target_include_directories(
    ${TARGET_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/exceptions/include
)