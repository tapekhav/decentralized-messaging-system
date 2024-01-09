set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

include("${CMAKE_CURRENT_BINARY_DIR}/conan_toolchain.cmake")

find_package(gRPC REQUIRED)
find_package(Protobuf REQUIRED)

file(GLOB PROTO_FILES "protos/*.proto")
set(SOURCE_PROTO_FILES ${PROTO_FILES})

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
    #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    #add_compile_options(/W4)
    #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MT")
endif()

file(GLOB PROTO_FILES "protos/*.proto")
set(SOURCE_PROTO_FILES ${PROTO_FILES})

set(output_files "")
get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
message(STATUS "grpc_cpp_plugin_location = ${grpc_cpp_plugin_location}")

set(protoc_location "/home/vladimir/.conan2/p/protod3b4d84d71378/p/bin/protoc")
message(STATUS "protoc_location=${protoc_location}")

foreach(proto_file ${SOURCE_PROTO_FILES})
    get_filename_component(output_file_name ${proto_file} NAME_WE)
    list(APPEND output_files
        ${CMAKE_CURRENT_BINARY_DIR}/protos/${output_file_name}.pb.cc
        ${CMAKE_CURRENT_BINARY_DIR}/protos/${output_file_name}.pb.h
        ${CMAKE_CURRENT_BINARY_DIR}/protos/${output_file_name}.grpc.pb.cc
        ${CMAKE_CURRENT_BINARY_DIR}/protos/${output_file_name}.grpc.pb.h
    )

    add_custom_command(
        OUTPUT
            ${CMAKE_CURRENT_BINARY_DIR}/protos/${output_file_name}.pb.cc
            ${CMAKE_CURRENT_BINARY_DIR}/protos/${output_file_name}.pb.h
            ${CMAKE_CURRENT_BINARY_DIR}/protos/${output_file_name}.grpc.pb.cc
            ${CMAKE_CURRENT_BINARY_DIR}/protos/${output_file_name}.grpc.pb.h
        COMMAND
            ${protoc_location}
            ARGS
                --cpp_out=${CMAKE_CURRENT_BINARY_DIR}/protos
                -I${CMAKE_CURRENT_SOURCE_DIR}/protos
                ${proto_file}
        COMMAND
            ${protoc_location}
            ARGS
                --grpc_out=${CMAKE_CURRENT_BINARY_DIR}/protos
                -I${CMAKE_CURRENT_SOURCE_DIR}/protos
                --plugin=protoc-gen-grpc=${grpc_cpp_plugin_location}
                ${proto_file}
        DEPENDS
            ${proto_file}
    )
endforeach()

add_custom_target(proto_target ALL DEPENDS ${output_files})

