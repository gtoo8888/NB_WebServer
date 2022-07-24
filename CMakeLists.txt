cmake_minimum_required(VERSION 3.10)
project(NB_webserve VERSION 0.1.0 LANGUAGES CXX)
# project(NB_webserve VERSION 0.1.0 )

set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_FLAGS -pthread)

# REQUIRED：可选字段。表示一定要找到包，找不到的话就立即停掉整个CMake。而如果不指定REQUIRED则CMake会继续执行。

# message(STATUS "CMAKE_PROJECT_NAME                  : ${CMAKE_PROJECT_NAME}")       # 非重要消息；
# message(WARNING "PROJECT_VERSION                    : ${PROJECT_VERSION}")          # CMake 警告, 会继续执行；
# message(AUTHOR_WARNING "PROJECT_VERSION_MAJOR       : ${PROJECT_VERSION_MAJOR}")    #  CMake 警告 (dev), 会继续执行；
# message(SEND_ERROR "PROJECT_VERSION_PATCH           : ${PROJECT_VERSION_PATCH}")    # CMake 错误, 继续执行，但是会跳过生成的步骤；
# message(FATAL_ERROR "PROJECT_VERSION_TWEAK          : ${PROJECT_VERSION_TWEAK}")    # CMake 错误, 继续执行，但是会跳过生成的步骤；



message(STATUS "CMAKE_MODULE_PATH = ${CMAKE_ROOT}")

file(GLOB_RECURSE LOG_SRC_LIST "/log/*.cpp" "/log/*h")
source_group(logserver FILES ${LOG_SRC_LIST})

add_executable(${PROJECT_NAME}
    ${LOG_SRC_LIST}
    main.cpp
    NB_config.cpp
)


# find_package(MySQL CONFIG REQUIRED)
# target_link_libraries(${PROJECT_NAME} PRIVATE gRPC::grpc++ gRPC::grpc gRPC::gpr)

include_directories(/usr/include/mysql)
target_link_libraries(${PROJECT_NAME} mysqlclient)



