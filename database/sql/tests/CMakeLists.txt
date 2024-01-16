set(TARGET_NAME TestSqlDB)

find_package(GTest)
if (${GTest_FOUND})
    add_compile_definitions(kMigrationPath="${CMAKE_CURRENT_SOURCE_DIR}/migrations/")
    include(GoogleTest)
    message(STATUS "Starting tests...")

    file(GLOB SOURCE_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
    add_executable(
        ${TARGET_NAME}
        main.cc 
        ${SOURCE_TESTS}
    )

    target_link_libraries(
        ${TARGET_NAME}
        core
        gtest
        gtest_main
        ${CMAKE_THREAD_LIBS_INIT} 
        GTest::GTest
        GTest::gtest_main
        GTest::gmock
        GTest::gmock_main
        PostgreSQL::PostgreSQL
        libpq::pgport
        libpq::pgcommon
        libpq::pq
        libpqxx::pqxx
    )

    include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../libs/core/include
    )
    enable_testing()

    gtest_discover_tests(${TARGET_NAME})
endif(${GTest_FOUND})