@echo off

set CM="C:\Program Files\CMake\bin\cmake.exe"

rem call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\Tools\VsDevCmd.bat"

if exist build rmdir /S /Q build
mkdir build
cd build

%CM% -G "Visual Studio 15" -Wdev ../

cd ..