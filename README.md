# YoAudio

####About

A C++ audio engine for games

##Overview

###Build Instructions

The following requirements need to be met to be able to build this project:

- SDL 2.0+ Development Libraries
- SDL_image 2.0+ Development Libraries

The build process has only ever been tested with **Microsoft Visual Studio 2017**. Follow these steps to generate a Visual Studio project:

- extract **SDL2** to `lib/SDL2`
- extract **SDL_image** to `lib/SDL_image`
- run **cmake**
	- on Windows simply execute `Windows-GenVisualStudioProject.bat`
- Open the solution at `build/YoAudio.snl`
- build by hitting `ctrl + shift + b`

cleaning up thye build folder can be accomplished by executing `Windows-Clean.bat`

###Playing a sound

Not implemented :P