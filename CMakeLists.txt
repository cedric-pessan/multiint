
cmake_minimum_required(VERSION 2.8)

project(multiinttest)

set(CMAKE_BUILD_TYPE Release)

add_subdirectory(/usr/src/gtest gtest)

add_executable(tests unit_tests.cpp)

target_link_libraries(tests gtest gtest_main gmp)