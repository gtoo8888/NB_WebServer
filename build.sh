#!/bin/bash

OUTPUT=PIGG_output
# 使用makefile编译
# chmod +x makefile
# make server


# 使用cmake编译
# echo "使用cmake编译"
if [ ! -d ${OUTPUT} ];then
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
# ./getCodeNum.sh   // 统计代码行数的

# ./build/PIGG_webserve



