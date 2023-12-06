set(TARGET_NAME core)

add_library(${TARGET_NAME})

file(GLOB HEADERS headers/*.h)
file(GLOB SOURCES src/*.cpp)

target_sources(
    ${TARGET_NAME}
    PUBLIC
    ${HEADERS} 
    ${SOURCES}
)