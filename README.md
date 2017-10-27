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

To see how sounds are being played back check out **main.cpp**.

Here is the basic idea:

	// initialize SDL audio
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		return 1;
	}

	// initialize YO audio
	YOA_Init();
	
	// Play Oneshot
	PlaySound("door_open_01.wav", false, SDL_MIX_MAXVOLUME / 2);
	
	// Play Loop and store Voice ID
	uint16_t ambLoop = PlaySound("ambience.wav", true, SDL_MIX_MAXVOLUME);

	// Stop sound using Voice ID
	StopVoice(ambLoop);

	// quit YO audio
	YOA_Quit();

	// quit SDL
	SDL_Quit();
	
	return 0;