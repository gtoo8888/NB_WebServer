cmake_minimum_required(VERSION 3.10)
project(NB_webserve)

set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_FLAGS -pthread)



add_executable(NB_webserve /
                main.cpp NB_config.cpp )