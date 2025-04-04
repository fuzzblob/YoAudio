#!/bin/sh

rm -rf build_CodeLite
mkdir build_CodeLite -p
cd build_CodeLite

cmake -G "CodeLite - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ../
cd ..