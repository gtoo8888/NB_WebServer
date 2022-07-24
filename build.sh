#!/bin/bash

OUTPUT=output
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
cmake .. -DCMAKE_BUILD_TYPE=Debug
make 
ls -l NB_webserve
cd -

# ./build/NB_webserve



