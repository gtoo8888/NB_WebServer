#!/bin/bash

# 使用makefile编译
# chmod +x makefile
# make server

使用cmake编译
mkdir build
cd build
cmake ..
make 
./NB_webserve
ls -l NB_webserve
cd -



