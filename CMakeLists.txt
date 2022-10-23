cmake_minimum_required(VERSION 3.10)
project(PIGG_webserve VERSION 0.1.0 LANGUAGES CXX)
# project(PIGG_webserve VERSION 0.1.0 )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread) # 开了创建线程什么的，一定要开这个

set(CXX_FLAGS
    -g
    # -MMD
    # -std=c++2a
    -rdynamic # 编译选项通知链接器将所有符号添加到动态符号表中。（目的是能够通过使用 dlopen 来实现向后跟踪）
    # -DVALGRIND
    -DCHECK_PTHREAD_RETURN_VALUE # 不明
    -D_FILE_OFFSET_BITS=64 # 不明
    -Wall   # 编译后显示所有警告
    -Wno-deprecated  # 不要警告使用已弃用的功能
    -Werror # 编译器视所有警告为错误，出现任何警告立即放弃编译
    -Wno-unused-function   # 关闭 函数被定义了但没有被使用 而产生的警告，即，不使用的函数不警告。
    -Wno-builtin-macro-redefined      # 如果某些内置宏被重新定义，请不要警告
 )


# add_compile_options(-fno-elide-constructors) #关闭编译器优化看起来没有什么用

add_definitions(-w) # 忽略警告

# add_compile_options命令添加的编译选项是针对所有编译器的(包括c和c++编译器)
# CMAKE_C_FLAGS或CMAKE_CXX_FLAGS变量则是分别只针对c和c++编译器

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

file(GLOB_RECURSE MYSQL_SRC_LIST "CGImysql/*.cpp" "CGImysql/*.h")
source_group(mysqlserver FILES ${MYSQL_SRC_LIST})

file(GLOB_RECURSE HTTP_SRC_LIST "PIGG_http/*.cpp" "PIGG_http/*.h")
source_group(httpserver FILES ${HTTP_SRC_LIST})

file(GLOB_RECURSE TIMER_SRC_LIST "PIGG_timer/*.cpp" "PIGG_timer/*.h")
source_group(timerserver FILES ${TIMER_SRC_LIST})

file(GLOB_RECURSE CONFIG_SRC_LIST "PIGG_config/*.cpp" "PIGG_config/*.h")
source_group(timerserver FILES ${CONFIG_SRC_LIST})

file(GLOB_RECURSE LOCKER_SRC_LIST "PIGG_locker/*.h")
source_group(lockerserver FILES ${LOCKER_SRC_LIST})

file(GLOB_RECURSE THREAD_SRC_LIST "PIGG_threadpool/*.h")
source_group(threadserver FILES ${THREAD_SRC_LIST})

add_executable(${PROJECT_NAME}
    ${LOG_SRC_LIST}
    ${TEST_SRC_LIST}
    ${LOCKER_SRC_LIST}
    ${MYSQL_SRC_LIST}
    ${HTTP_SRC_LIST}
    ${CONFIG_SRC_LIST}
    ${TIMER_SRC_LIST}
    ${THREAD_SRC_LIST}
    main.cpp
    PIGG_webserver.cpp
)


# find_package(MySQL CONFIG REQUIRED)
# target_link_libraries(${PROJECT_NAME} PRIVATE gRPC::grpc++ gRPC::grpc gRPC::gpr)

# MySQL
include_directories(/usr/include/mysql)
target_link_libraries(${PROJECT_NAME} mysqlclient)

# jsoncpp
find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} jsoncpp_static)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/release)   # 设置可执行文件的位置
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/release)      # 设置动态链接库的位置

# include_directories(/usr/local/include/json)
# target_link_libraries(${PROJECT_NAME} mysqlclient)

# set(jsoncpp_DIR /usr/local/lib/)
# find_package(jsoncpp REQUIRED)
# pkg_check_modules(JSONCPP jsoncpp)
# include_directories(${JSONCPP_LIBRARIES})
# target_link_libraries(${PROJECT_NAME} ${JSONCPP_LIBRARIES})


# set(lib_DIR /usr/lib/x84_64-linux-gnu)
# link_directories(${lib_DIR})
# target_link_libraries(${PROJECT_NAME} libjsoncpp.a)


# set(lib_DIR /usr/lib/x84_64-linux-gnu)
# find_package(${lib_DIR})
# target_link_libraries(${PROJECT_NAME} libjsoncpp.a)


# Qt5
# find_package(Qt5 COMPONENTS Core Network Concurrent REQUIRED) 
# target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::CorePrivate Qt5::Network Qt5::Concurrent)


# add_executable(PIGG_webserve /
#                 main.cpp PIGG_config.cpp )


# install(TARGETS ${PROJECT_NAME}
#         CONFIGURATIONS Debug
#         RUNTIME DESTINATION Debug/bin)
# install(TARGETS ${PROJECT_NAME}
#         CONFIGURATIONS Release
#         RUNTIME DESTINATION Release/bin)

# 暂时先不安装，减少报错量，不然两遍报错
# install(TARGETS ${PROJECT_NAME}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}     # 可执行二进制文件
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}     # 动态库
#     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}     # 静态库
#     OBJECTS DESTINATION ${CMAKE_INSTALL_LIBDIR}
# )

