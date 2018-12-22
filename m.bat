@echo off

rem set CMake executable path
set CM="C:\Program Files\CMake\bin\cmake.exe"

rem call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"

rem clear or make build directory 
if exist build rmdir /S /Q build
mkdir build
cd build

set VS150COMNTOOLS = "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\"
rem run CMake
rem change the target project type if not using VisualStudio
rem apend Win64 to build 64bit binaries
%CM% -G "Visual Studio 15 2017" -Wdev ../

cd ..