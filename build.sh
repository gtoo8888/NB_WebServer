#!/bin/bash

# 使用makefile编译
# chmod +x makefile
# make server

使用cmake编译
mkdir output
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make 
ls -l NB_webserve
cd -

# ./build/NB_webserve



