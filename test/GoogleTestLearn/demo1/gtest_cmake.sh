#! /bin/bash


# cmake -S . -B build


if [ ! -d "build"];then
    mkdir build
fi

mkdir build 
cd build
cmake ..
make -j all
cd build
./demo1
