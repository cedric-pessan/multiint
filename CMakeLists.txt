
cmake_minimum_required(VERSION 2.8)

project(multiinttest)

set(CMAKE_BUILD_TYPE Release)

add_subdirectory(gtest/googletest)
enable_testing()

add_definitions(-std=c++11)
include_directories(gtest/googletest/include)
add_executable(tests unit_tests.cpp)

target_link_libraries(tests gtest gtest_main gmp)