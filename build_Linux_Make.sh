#!/bin/sh

rm -rf build
mkdir build -p
cd build

cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ../
cmake --build . --target YoAudio --config Debug
