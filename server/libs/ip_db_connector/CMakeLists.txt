set(TARGET_NAME ip_db_connector)

file(GLOB HEADERS include/*.h)
file(GLOB SOURCES src/*.cpp)

add_library(${TARGET_NAME} ${SOURCES})

target_include_directories(
    ${TARGET_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(
    ${TARGET_NAME}
    PUBLIC
    ${HEADERS} 
    ${SOURCES}
)