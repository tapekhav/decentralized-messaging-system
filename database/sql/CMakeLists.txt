cmake_minimum_required(VERSION 3.20)
project(DecentralizedMessagingSystemSQL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "./build")

find_package(ZeroMQ REQUIRED)
find_package(gRPC REQUIRED)
find_package(SOCI REQUIRED)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG ("-std=c++20" COMPILER_HAVE_CPP20)
if (COMPILER_HAVE_CPP20)
    add_compile_options("-std=c++20")
else()
    message(FATAL_ERROR "Compiler does not support C++20")
endif()

if (MSVC_VERSION GREATER_EQUAL "1900")
    CHECK_CXX_COMPILER_FLAG("/std:c++20" _cpp_20)
    if (_cpp_20)
        message(STATUS "ADDING C++20")
        add_compile_options("/std:c++20")
    endif()
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    #add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wformat -Wunused -Wshadow -Wuninitialized)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    #add_compile_options(/W4)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MT")
endif()

include_directories(
    ${gRPC_INCLUDE_DIR}
    ${PROTOBUF_INCLUDE_DIRS}
)

add_subdirectory("libs/")

add_executable(
    ${PROJECT_NAME}
    main.cpp
)

target_link_libraries(
    ${PROJECT_NAME} 
    core
    ${ZeroMQ_LIBRARIES}
    ${_GRPC_LIBRARIES}
    SOCI::soci_core_static
)

