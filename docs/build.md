# [YoAudio](../README.md)
[![Build Status](https://travis-ci.org/fuzzblob/YoAudio.svg?branch=master)](https://travis-ci.org/fuzzblob/YoAudio)  [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

- [Setting Up](#setup)
	- [Prerequisites](#prerequisites)
- [Build Instructions](#build)
	- [Building on Windows](#windows)
	- [Building on Linux](#linux)
	- [Building on Mac OS](#macos)
- [On git submodules](#submodules)

## Setting Up <a name="setup"></a>

### Prerequisites <a name="prerequisites"></a>

- A modern install of [git](https://git-scm.com/downloads)
- An modern installation of [CMake 3.8 or newer](https://cmake.org/)
- SDL2 ([Simple DirectMedia Layer](https://www.libsdl.org/), [prebuilt downloads](https://github.com/libsdl-org/SDL/releases))
	- SDL2 is being used as the audio rendering backend in the Engine, and for rendering graphics in the Editor.
	- SDL3 has been released, but not tested yet.
- clone the repository with submodules:
	- **[submodule]** The YoAudio Engine uses [sdplog](https://github.com/gabime/spdlog) for printing to the console (not included in Release builds). It is an optional **git submodule**.
	- **[submodule]** The YoAudio Editor uses [imgui](https://github.com/ocornut/imgui) for rendering immediate mode UI elements. It is required for the editor to compile and function as intended.
	- For more information about the submodule workflow see below at [On git submodules](#submodules)

```Shell
git clone --recurse-submodules https://github.com/fuzzblob/YoAudio.git
```

## Build Instructions <a name="build"></a>

### Building on Windows <a name="windows"></a>

At the moment the build process has only ever been tested on Windows with Microsoft Visual Studio 2017 Community Edition. Follow these steps to generate a Visual Studio project:

- install Microsoft Visual Studio (currently recommended [VS17 2022](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community&channel=Release&version=VS2022))
	- in the setup select the MSVC C++ compiler and Windows SDK
- download the [SDL2 Development Libraries](https://github.com/libsdl-org/SDL/releases/download/release-2.32.2/SDL2-devel-2.32.2-VC.zip) (zip file labeled **"SDL2-devel-2.XX.X-VC"**)
	- extract the `include` and `lib` folders to `./dependencies/SDL2/`
- run CMake
	- an example is provided for VS 17 2022 [build_Windows_VisualStudio2022.bat](../build_Windows_VisualStudio2022.bat)
- open the created solution file (e.g. `/build/YoAudio.sln`) and build the code (`Ctrl + Shift + B`)
- the output will be built to `/bin/Debug`
	- YoAudio-Engine.lib (linking symbols)
	- YoAudio-Engine.dll (runtime)
	- YoAudio-Engine.pbd (debug symbols)
	- YaAudio-Editor.exe (executable editor programm)

### Building on Linux <a name="linux"></a>

#### dependencies

- Ubuntu / Debian users run: `sudo apt-get install build-essential cmake ninja-build libsdl2-dev`
    - you can now skip down to the build step

To build the project yourself you will need the following:

- A modern **C** and **C++** compiler:
    - version 13+ of `gcc`
    - version 20+ of `clang` and `clang-tools`)
- A build tool such as `make` or `ninja` in addition to `cmake`
- The SDL2 development libraries:
    - Arch: `pacman -S sdl2`
    - Ubuntu / Debian: `apt-get install libsdl2-dev`
    - Fedora: `dnf install SDL2-devel`
    - Gentoo: `emerge media-libs/libsdl2`

#### building the project

- An easyto use build script is available at [./build-scripts/linux.sh](../build-scripts/linux.sh)
    - either call up the help text with `./linux.sh --help` or let it auto-detect the available tools
- the output will be built to `/bin/` as **libYoAudio.so** & **YOA_Editor**

To run the Editor software you will also need:
- an ALSA compatible audio backend (`alsa-base` or `pipewire`)
- a graphical user interface


### Building on Mac OS <a name="macos"></a>

As I don't have a Mac OS build environment I can not provide detailed instructions on how to build YoAudio for that platform. A pull request with a build script, as well as any changes required to *CMakeLists.txt* and source code would be much appreciated. What follows is an assumption:

- install SDL via [homebrew](https://formulae.brew.sh/formula/sdl2) (*CMakeLists.txt* is setup to find it in the default install directory)
    - `brew install sdl2`
- run CMake
    - chances are the new `./build-scripts/linux.sh` will work as expected 
- build

## On Submodules <a name="submodules"></a>

YoAudio uses a git feature called submodules to tie in other git repositories as dependencies. If you don't want to or can't use this feature, you will have to make the dependencies work by hand (adding files to `engine/dependencies/` and `editor/dependencies`, passing values to CMake, possibly editing *CMakeLists.txt*). This can be tricky as the *CMake* scripts require specific folder locations to detect the dependencies and set up the libraries properly for the compilation of code to function.
To clone the repository with submodules paste the following command in your terminal

```Shell
git clone --recurse-submodules https://github.com/fuzzblob/YoAudio.git
```
If you accidentally cloned the repository without the `--recurse-submodules` option, you can fetch them after the fact with the following command:

```Shell
git submodule update --init --recursive --remote
```

Sadly this will (probably) pull in the latest versions of the submodules, instead of the ones specified by the YoAudio repository, but it may well work.
Alternatively use a git client that supports git submodules like [Tortoise Git](https://tortoisegit.org/) or [SourceTree](https://www.sourcetreeapp.com/).

- [git submodule documentation](https://git-scm.com/book/en/v2/Git-Tools-Submodules)
- [how to clone submodules](https://stackoverflow.com/questions/3796927/how-to-git-clone-including-submodules))
