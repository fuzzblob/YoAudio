# YoAudio

- [About](#about)
- [Overview](#overview)
	- [Playing a sound](#sound)
	- [Dependencies](#dependencies)
- [Build Instructions](#build)
- [TODO](#todo)
- [Acknowledgements](#acknowledge)

### About <a name="about"></a>

A C++ audio environment meant as a playground for experimentation with game audio concepts and DSP algorithms.

WARNING! YoAudio is not release quality!

**Goals**

- develop a usable audio framework for games (and other applications)
- learn C++ development: low level, high performance, flexible and robust audio plaback for games as well as digital signal processing (DSP)

## Overview <a name="overview"></a>

### Playing a sound <a name="sound"></a>

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

### Dependencies <a name="dependencies"></a>

- Simple DirectMedia Layer [download SDL Development Libraries 2.0.x (stable)](http://libsdl.org/download-2.0.php)
	- SDL2 is being used as the audio rendering backend (potentially others will be added later)
- [submodule] [sdplog](https://github.com/gabime/spdlog)
	- this submodule is optional (logging will be disabled if not present during CMake build)

On Submodules:
- [git documentation](https://git-scm.com/book/en/v2/Git-Tools-Submodules)
- [how to clone submodules](https://stackoverflow.com/questions/3796927/how-to-git-clone-including-submodules))

## Build Instructions <a name="build"></a>

### Windows

At the moment the build process has only ever been tested on Windows with Microsoft Visual Studio 2017 Community Edition. Follow these steps to generate a Visual Studio project:

- clone repo using `git clone --recurse-submodules -j8 https://github.com/fuzzblob/YoAudio.git`
	- alternatively use a git client that supports git submodules (see [Dependencies section](#dependencies))
	- alternatively clone the submodule dependencies seperately and copy them to `dependencies/`
		- this is finicky as the CMake scripts require specific folder locations
- download the **SDL2** development libraries (see [Dependencies](#dependencies))
	- extract (at least) the *include* & *lib* directories to `dependencies/SDL2`
- to run [**cmake**](https://cmake.org/) on Windows simply execute `m.bat` (assuming it's installed)
	- if not using VisualStudio 2017 you might have to futz with *CMakeLists.txt*
- Open the solution at `build/YoAudio.snl`
- build (by hitting `ctrl + shift + b`)
- the output will be built to `bin/Debug` or `bin/Release`
	- YoAudio.dll for runtime
	- YoAudio.pdb for debug symbols
- if you want a quick and easy sandbox to test stuff in clone the [YoAudio Editor repo](https://github.com/fuzzblob/YoAudioEditor) to the same folder as the YoAudio repo was cloned into and build that. It will find the YoAudio sources, and copy the built *YoAudio.dll* so it can run.

### Linux & Mac OS

If you build this project on these platforms, let me know how it went. If you have to make changes to *CMakeLists.txt* or the source code, please submit a pull request so I can try get your changes into the main repo.

- when using Mac OS you can install SDL via homebrew and CMake **should** pick it up

## TODO <a name="todo"></a>

These are things I'd like to explore with this library / tool in the future (in no particular order):

- add resampling of WAV data (spec to device spec)
	- pitch algorithm needs filter
- Voice management
	- enforce voice limit
		- virtual state: tracking playback position without mixing
	- get a collection of voices currently playing via Mixer::GetPlayingVoices()
	- stop all voices playing the sound via Mixer::StopSound()
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

## Acknowledgements<a name="acknowledge"></a>

The initial development was heavily inspired by this [Simple SDL2 Audio example](https://github.com/jakebesworth/Simple-SDL2-Audio).