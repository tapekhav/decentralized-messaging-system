cmake_minimum_required(VERSION 3.20)
project(DecetralizedMessagingSystem)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")

find_package(ZeroMQ REQUIRED)

include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG ("-std=c++20" COMPILER_HAVE_CPP20)
if (COMPILER_HAVE_CPP20)
    set(CMAKE_CXX_STANDARD 20)
endif(COMPILER_HAVE_CPP20)

if (MSVC_VERSION GREATER_EQUAL "1900")
    CHECK_CXX_COMPILER_FLAG("/std:c++20" _cpp_20)
    if (_cpp_20)
        message(STATUS "ADDING C++20")
        add_compile_options("/std:c++20")
    endif()
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Werror -Wpedantic -Wconversion -Wformat -Wunused -Wshadow -Wuninitialized -Wsign-compare)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MT")
endif()

file(GLOB PROTO_FILES "protos/*.proto")
set(SOURCE_PROTO_FILES ${PROTO_FILES})

set(output_files "")

foreach(proto_file ${SOURCE_PROTO_FILES})
    get_filename_component(output_file_name ${proto_file} NAME_WE)
    list(APPEND output_files
        ${CMAKE_CURRENT_BINARY_DIR}/protos/${output_file_name}.pb.cc
        ${CMAKE_CURRENT_BINARY_DIR}/protos/${output_file_name}.pb.h
    )

    add_custom_command(
        OUTPUT
            ${CMAKE_CURRENT_BINARY_DIR}/protos/${output_file_name}.pb.cc
            ${CMAKE_CURRENT_BINARY_DIR}/protos/${output_file_name}.pb.h
        COMMAND
            protoc
            ARGS
                --cpp_out=${CMAKE_CURRENT_BINARY_DIR}/protos
                -I${CMAKE_CURRENT_SOURCE_DIR}/protos
                ${proto_file}
        DEPENDS
            ${proto_file}
    )
endforeach()

add_custom_target(proto_target ALL DEPENDS ${output_files})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/protos)

add_subdirectory("libs/")

add_executable(
    ${PROJECT_NAME}
    main.cpp
)
target_link_libraries(
    ${PROJECT_NAME} 
    PRIVATE
    core
    ${ZeroMQ_LIBRARIES}
)
