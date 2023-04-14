cmake_minimum_required(VERSION 3.10)
project(demo1)

set(CMAKE_CXX_STANDARD 14)

add_executable(demo1 gtest_learn_main.cpp demo1.cpp demo1.h demo1_unitest.cpp)

target_link_libraries(demo1 gtest gtest_main pthread) # 一定需要加上pthread

include(GoogleTest)
# getest_discover_tests(demo1)


