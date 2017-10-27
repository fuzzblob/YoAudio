# YoAudio

- [About](#about)
- [Overview](#overview)
- [Build Instructions](#build)
- [Playing a sound](#sound)

### About <a name="about"></a>

A C++ audio environment. It is meant as a playground for experimentation with game audio concepts and DSP algorithms.

## Overview <a name="overview"></a>

### Build Instructions <a name="build"></a>

The following requirements need to be met to be able to build this project:

- SDL 2.0+ Development Libraries
- SDL_image 2.0+ Development Libraries

### Windows

At the moment the build process has only ever been tested on Windows with Microsoft Visual Studio 2017. Follow these steps to generate a Visual Studio project:


- extract **SDL2** to `lib/SDL2`
- extract **SDL_image** to `lib/SDL_image`
- run **cmake**
	- on Windows simply execute `Windows-GenVisualStudioProject.bat`
- Open the solution at `build/YoAudio.snl`
- build by hitting `ctrl + shift + b`
- to remove the Solution or re-generate it you should first run `Windows-Clean.bat`

### Linux & Mac OS

to be determined

## Playing a sound <a name="sound"></a>

Not implemented :P
