cmake_minimum_required(VERSION 3.10.0)
project(rubber-duck)

# Make sure developers do not run cmake in the main project directory, to keep
# build artifacts from becoming clutter
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed.
    Please make a new directory (called a build directory) and run CMake from there.
    You may need to remove CMakeCache.txt." )
endif()

# Turn on debug builds if we are building in a devel shell
if (CMAKE_BUILD_TYPE STREQUAL "" AND "$ENV{DEVEL_SHELL}" STREQUAL "1")
    message(STATUS "Setting debug build type by default in devel shell")
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Output binaries to a sub directory "bin"
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable cmake testing
include(CTest)
enable_testing()

# Enable GoogleTest
include(GoogleTest)
add_subdirectory(/usr/src/googletest googletest)

# Enable Boost
# Use static libraries so binaries can be deployed without a full boost install
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.50 REQUIRED COMPONENTS system thread)
message(STATUS "Boost version: ${Boost_VERSION}")

# include .h files
include_directories(include)

# create libraries
add_library(server_lib src/http/server/server.cc src/http/server/session.cc src/http/server/request_parser.cc src/http/server/header.cc src/http/server/reply.cc src/http/server/request_handler.cc)
add_library(config_parser_lib src/nginx/config_parser.cc src/nginx/config.cc src/nginx/config_statement.cc)

# create executable of the server program
add_executable(server src/http/server/server_main.cc)
target_link_libraries(server server_lib config_parser_lib Boost::system)

# add executables for tests
add_executable(server_test tests/http/server/server_test.cc)
target_link_libraries(server_test server_lib Boost::system gtest_main)
add_executable(request_parser_test tests/http/server/request_parser_test.cc)
target_link_libraries(request_parser_test server_lib gtest_main Boost::system)
add_executable(reply_test tests/http/server/reply_test.cc)
target_link_libraries(reply_test server_lib gtest_main Boost::system)
add_executable(request_handler_test tests/http/server/request_handler_test.cc)
target_link_libraries(request_handler_test server_lib gtest_main Boost::system)
add_executable(config_parser_test tests/nginx/config_parser_test.cc)
target_link_libraries(config_parser_test config_parser_lib gtest_main)

# find google tests
gtest_discover_tests(server_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(config_parser_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(request_parser_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(reply_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(request_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# Update with target/test targets
include(cmake/CodeCoverageReportConfig.cmake)
generate_coverage_report(TARGETS server server_lib config_parser_lib TESTS server_test config_parser_test request_parser_test reply_test request_handler_test)
