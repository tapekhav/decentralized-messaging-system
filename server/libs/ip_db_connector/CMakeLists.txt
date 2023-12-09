set(TARGET_NAME ip_db_connector)

add_library(${TARGET_NAME})

file(GLOB HEADERS include/*.h)
file(GLOB SOURCES src/*.cpp)

target_sources(
    ${TARGET_NAME}
    PUBLIC
    ${HEADERS} 
    ${SOURCES}
)