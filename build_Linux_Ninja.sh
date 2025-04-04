#!/bin/sh

echo "removing, creating and moving to build dir..."
#rm -rf build_ninja
mkdir build_ninja -p
cd build_ninja

echo "\nmaking DEBUG unix makefiles:"
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Debug ../
echo "\nbuilding DEBUG build of YoAudio:"
#cmake --build . --target YoAudio --config Debug
ninja -d stats
cd ..
