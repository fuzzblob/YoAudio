#!/bin/sh

echo "removing, creating and moving to build dir..."
#rm -rf build_make
mkdir build_make -p
cd build_make

echo "\nmaking DEBUG unix makefiles:"
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ../
echo "\nbuilding DEBUG build of YoAudio:"
#cmake --build . --target YoAudio --config Debug
make
cd ..
