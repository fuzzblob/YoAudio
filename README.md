# YoAudio

- [About](#about)
- [Overview](#overview)
	- [Dependencies](#dependencies)
	- [Build Instructions](#build)
- [Playing a sound](#sound)
- [TODO](#todo)

### About <a name="about"></a>

A C++ audio environment. It is meant as a playground for experimentation with game audio concepts and DSP algorithms. The initial development was heavily inspired by this [Simple SDL2 Audio example](https://github.com/jakebesworth/Simple-SDL2-Audio).

**Goals**

- develop a usable audio framework for games (and other applications)
- learn C++ development: low level, high performance, flexible and robust audio plaback for games as well as digital signal processing (DSP)

## Overview <a name="overview"></a>

### Dependencies <a name="dependencies"></a>

- Simple DirectMedia Layer [SDL version 2.0.7 (stable)](http://libsdl.org/download-2.0.php)
- (sdplog)[https://github.com/gabime/spdlog] (as a (git submodule)[https://git-scm.com/book/en/v2/Git-Tools-Submodules])

### Build Instructions <a name="build"></a>

The following requirements need to be met to be able to build this project:

- SDL 2.0+ Development Libraries
- spdlog needs to be present in your project (either by using a submodule compatible client or by cloning it to "dependencies\spdlog")

### Windows

At the moment the build process has only ever been tested on Windows with Microsoft Visual Studio 2017 Community Edition. Follow these steps to generate a Visual Studio project:

- extract **SDL2** to `dependencies/SDL2` (see #dependencies)
	- make sure to copy *include*, *lib* and *bin* directories
- to run (**cmake**)[https://cmake.org/] on Windows simply execute `m.bat` (assuming it's installed)
- Open the solution at `build/YoAudio.snl`
- build (by hitting `ctrl + shift + b` if using Visual Studio)
- the output will be built to `bin/Debug` or `bin/Release`
	- YoAudio.dll for runtime
	- YoAudio.pdb for debug symbols
- if you want a quick and easy sandbox to test stuff in clone the (YoAudio Editor repo)[https://github.com/fuzzblob/YoAudioEditor] to the same folder as the YoAudio repo was cloned into and build that. It will find the YoAudio sources, and copy the built *YoAudio.dll* so it can run.

### Linux & Mac OS

If you build this project on these platforms, let me know how it went. If you have to make changes to *CMakeLists.txt* or the source code, please submit a pull request so I can try get your changes into the main repo.

## Playing a sound <a name="sound"></a>

To see how sounds are being played back check out **main.cpp**.

Here is the basic idea:

	#include "YoAudio.h"

	int main()
	{
		// initialize YO audio
		YOA_Init();
		
		float volume = 1.0f;
		float pitch = 1.0f;
		float fadeTime = 0.75f
		
		// Play Oneshot
		YOA_PlaySound("door_open_01.wav", false, volume, pitch, 0.0f);
		
		// Play loop and store VoiceID
		uint16_t voiceID = PlaySound("ambience.wav", true, volume, pitch, fadeTime);

		// Stop loop using VoiceID
		YOA_StopVoice(voiceID, fadeTime);
		// when stopping a voice with a fade time of 0.0f
		// the audio engine will set a 10ms fade to avoid artifacts

		// pauses the entire audio engine
		YOA_Pause();
		// resumes the audio engine
		YOA_Resume();

		// quit YO audio
		YOA_Quit();
		
		return 0;
	}

## TODO <a name="todo"></a>

These are things I'd like to explore with this library / tool in the future (in no particular order):

- Voice management
	- limit concurrently playing voices
		- add virtual state (tracking playback position without adding samples to the mix)
	- remove voices from playing vector when stopped
- add resampling of WAV data (spec to device spec)
	- proper pitch algorithm
- instead of hard coded 16bit int
	- convert from WAV spec format to float
	- convert from float to Device spec format
- add Audio Graph	
	- AudioListener
	- MixerGroups
	- add SampleGenerator interface
		- generalize audio sources (sample playback, synthesis, streaming)
		- GenerateSamples()
		- SamplesRemaining()
		- AudioEffect
			- limiter for final output
			- Filter (HighPass & LowPass)
- multithreading
	- create MessageQueue
	- create AudioRenderer
		- fill circular buffer of AudioFrames for the callback to consume (copy out of)