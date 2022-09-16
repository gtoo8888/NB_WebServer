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
cd -

if [ "$1"x == "len"x ];then # x为了防止变量为空时报错
    ./getCodeNum.sh   # 统计代码行数
fi

# ./build/PIGG_webserve



