#!/bin/sh

echo "removing, creating and moving to build dir..."
rm -rf build
mkdir build -p
cd build

echo "\nmaking DEBUG unix makefiles:"
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ../
echo "\nbuilding DEBUG build of YoAudio:"
cmake --build . --target YoAudio --config Debug
