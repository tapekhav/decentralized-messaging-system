
set(TARGET_NAME TestRedisDB)

find_package(GTest)
if (${GTest_FOUND})
    message(STATUS "Starting tests...")

    file(GLOB SOURCE_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
    add_executable(
        ${TARGET_NAME}
        main.cc 
        ${SOURCE_TESTS}
    )

    target_link_libraries(
        ${TARGET_NAME}
        utilities
        core
        gtest
        gtest_main
        ${CMAKE_THREAD_LIBS_INIT} 
        GTest::GTest
        GTest::gtest_main
        GTest::gmock
        GTest::gmock_main
        hiredis::hiredis
    )

    include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../libs/core/include
    )
    enable_testing()
    add_test(NAME ExampleTest COMMAND ${TARGET_NAME})
endif(${GTest_FOUND})