#!/bin/sh

rm -rf build
mkdir build -p
cd build

cmake -G "CodeLite - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ../
