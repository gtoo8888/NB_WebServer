#!/bin/bash

OUTPUT=PIGG_output
# 使用makefile编译
# chmod +x makefile
# make server


# 使用cmake编译
# echo "使用cmake编译"
if [ ! -d ${OUTPUT} ];then
    mkdir ${OUTPUT}
else
    rm -rf ${OUTPUT}         # 调试的时候使用
    mkdir ${OUTPUT}
fi

if [ ! -d build ];then
    mkdir build
fi

cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=.
make -j${nproc}
# make install
ls -lh PIGG_webserve
ls -l PIGG_webserve
cd -

if [ "$1"x == "len"x ];then # x为了防止变量为空时报错
    ./getCodeNum.sh   # 统计代码行数
else   
    echo "如果需要统计代码行数: ./build.sh len"
fi

if [ "$1"x == "start"x ];then # x为了防止变量为空时报错
    ./bin/release/PIGG_webserve   # 统计代码行数
else   
    echo "如果需要运行代码: ./build.sh start"
fi

# 只有在根目录下./build/PIGG_webserve不会报错
# 在build目录下,./PIGG_webserve使用时候fopen()会因为找不到路径而无法写日志



