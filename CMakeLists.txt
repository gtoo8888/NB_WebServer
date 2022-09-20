cmake_minimum_required(VERSION 3.10)
project(PIGG_webserve VERSION 0.1.0 LANGUAGES CXX)
# project(PIGG_webserve VERSION 0.1.0 )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread) # 开了创建线程什么的，一定要开这个

add_definitions(-w) # 忽略警告
# REQUIRED：可选字段。表示一定要找到包，找不到的话就立即停掉整个CMake。而如果不指定REQUIRED则CMake会继续执行。

# message(STATUS "CMAKE_PROJECT_NAME                  : ${CMAKE_PROJECT_NAME}")       # 非重要消息；
# message(WARNING "PROJECT_VERSION                    : ${PROJECT_VERSION}")          # CMake 警告, 会继续执行；
# message(AUTHOR_WARNING "PROJECT_VERSION_MAJOR       : ${PROJECT_VERSION_MAJOR}")    #  CMake 警告 (dev), 会继续执行；
# message(SEND_ERROR "PROJECT_VERSION_PATCH           : ${PROJECT_VERSION_PATCH}")    # CMake 错误, 继续执行，但是会跳过生成的步骤；
# message(FATAL_ERROR "PROJECT_VERSION_TWEAK          : ${PROJECT_VERSION_TWEAK}")    # CMake 错误, 继续执行，但是会跳过生成的步骤；



message(STATUS "CMAKE_MODULE_PATH = ${CMAKE_ROOT}")

file(GLOB_RECURSE LOG_SRC_LIST "PIGG_log/*.cpp" "PIGG_log/*.h")  # 不能写成/PIGG_log/*.cpp这样成了在根目录下寻找
source_group(logserver FILES ${LOG_SRC_LIST})

file(GLOB_RECURSE TEST_SRC_LIST "PIGG_test/*.cpp" "PIGG_test/*.h")
source_group(testserver FILES ${TEST_SRC_LIST})

file(GLOB_RECURSE LOCKER_SRC_LIST "PIGG_locker/*.cpp" "PIGG_locker/*.h")
source_group(testserver FILES ${LOCKER_SRC_LIST})

file(GLOB_RECURSE MYSQL_SRC_LIST "CGImysql/*.cpp" "CGImysql/*.h")
source_group(mysqlserver FILES ${MYSQL_SRC_LIST})

file(GLOB_RECURSE HTTP_SRC_LIST "PIGG_http/*.cpp" "PIGG_http/*.h")
source_group(httpserver FILES ${HTTP_SRC_LIST})

file(GLOB_RECURSE TIMER_SRC_LIST "PIGG_timer/*.cpp" "PIGG_timer/*.h")
source_group(timerserver FILES ${TIMER_SRC_LIST})

add_executable(${PROJECT_NAME}
    ${LOG_SRC_LIST}
    ${TEST_SRC_LIST}
    ${LOCKER_SRC_LIST}
    ${MYSQL_SRC_LIST}
    ${HTTP_SRC_LIST}
    ${TIMER_SRC_LIST}
    main.cpp
    PIGG_config.cpp
    PIGG_webserver.cpp
)


# find_package(MySQL CONFIG REQUIRED)
# target_link_libraries(${PROJECT_NAME} PRIVATE gRPC::grpc++ gRPC::grpc gRPC::gpr)

# MySQL
include_directories(/usr/include/mysql)
target_link_libraries(${PROJECT_NAME} mysqlclient)

# Qt5
# find_package(Qt5 COMPONENTS Core Network Concurrent REQUIRED) 
# target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::CorePrivate Qt5::Network Qt5::Concurrent)


# add_executable(PIGG_webserve /
#                 main.cpp PIGG_config.cpp )


install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS Debug
        RUNTIME DESTINATION Debug/bin)
install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS Release
        RUNTIME DESTINATION Release/bin)

# 暂时先不安装，减少报错量，不然两遍报错
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}     # 可执行二进制文件
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}     # 动态库
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}     # 静态库
    OBJECTS DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

