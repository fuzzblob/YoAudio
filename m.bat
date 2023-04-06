@echo off

set CM="C:\Program Files\CMake\bin\cmake.exe"

rem call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"

if exist build rmdir /S /Q build
mkdir build
cd build

set VS150COMNTOOLS = "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\"

%CM% -G "Visual Studio 17 2022" -A x64 -Wdev ../

cd ..

SET /P variable=["Press any key to exit..."]