@echo off

rem set CMake executable path
set CM="C:\Program Files\CMake\bin\cmake.exe"

rem clear or make build directory 
rem if exist build_ninja rmdir /S /Q build_ninja
mkdir build_ninja
rem clear or make bin directory 
if exist bin rmdir /S /Q bin
cd build_ninja

rem run CMake
%CM% -G "Ninja" -Wdev ../ -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

ninja.exe -d stats

cd ..