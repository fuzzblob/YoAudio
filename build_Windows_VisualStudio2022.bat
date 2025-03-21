@echo off

rem set CMake executable path
set CM="C:\Program Files\CMake\bin\cmake.exe"

rem call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"

rem clear or make build directory 
if exist build_MSVC rmdir /S /Q build_MSVC
mkdir build_MSVC
rem clear or make bin directory 
if exist bin rmdir /S /Q bin
cd build_MSVC

set VS150COMNTOOLS = "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\"
rem run CMake
rem change the target project type if not using VisualStudio
%CM% -G "Visual Studio 17 2022" -A x64 -Wdev ../

cd ..