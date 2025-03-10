![image: fox with headphones](misc/fox_small.png)

# YoAudio
[![Build Status](https://travis-ci.org/fuzzblob/YoAudio.svg?branch=master)](https://travis-ci.org/fuzzblob/YoAudio)  [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

- [About](#about)
- [Overview](#overview)
	- [Usage](#usage)
	- [Supported Formats](#formats)
	- [Example Code](#code)
- [Build Instructions](#build)
	- [Dependencies](#dependencies)
	- [Building on Windows](#windows)
	- [Building on Linux](#linux)
	- [Building on Mac OS](#macos)
- [TODO](#todo)
- [Acknowledgements](#acknowledge)

## About <a name="about"></a>

YoAudio is a C++ audio environment meant as a playground for experimentation with game audio concepts and DSP algorithms.

The name Yo is based on the phonetic approximation of the korean word for fox (여우).

**WARNING! YoAudio is not release quality!**

## Overview <a name="overview"></a>

### Using YoAudio <a name="usage"></a>

To use YoaAudio in your project the first step is to call `YOA_Init()` to initialize the audio engine. After that you can call `YOA_PlayWavFile(filename, loop, volume, pitch, fadeIn, pan)` to play WAV files.

The `YOA_PlayWavFile()` method returns a VoiceID which can be used to Stop a playing voice (`YOA_StopVoice(id, fadeOut)`), change it's volume (`YOA_SetVoiceVolume(id, newVolume)`) and panning(`YOA_SetVoicePan(id, newPan)`).

If your application goes out of context and you wish to pause audio rendering use `YOA_Pause()` & `YOA_Resume()`.

Once you wish to shut down the audio engine simply call `YOA_Quit(quitSDL)` passing a bool to signal if YoAudio, or your application will hande the Quitting of the SDL library.

### Supported playback formats <a name="formats"></a>

The audio data can be at any arbitrary sample rate. The audio mixer will resample the data if needed. The current implementation uses linear interpolation for resampling.

- WAV  8bit (unsigned integer)
- WAV 16bit (signed integer)
- WAV 32bit (signed integer)
- WAV 32bit IEEE-754 (float)

Planned future support (in order of importance):

- OGG Vorbis (encapsulated Opus)
- ADPCM
- MP3
- WAV 24bit (signed integer)

### Example Code  <a name="code"></a>

	#include <stdlib.h>
	#include "YoAudio.h"

	int main()
	{
		// initialize YO audio
		YOA_Init();
		
		// Play Oneshot
		// const char * fn, bool loop, float volume, float pitch, float fadeIn, float pan
		YOA_PlayWavFile("door_open_01.wav", false, 0.4f, 0.95f, 0.2f, 1.0f);
		
		// Play loop and store VoiceID
		uint16_t voiceID = YOA_PlayWavFile("ambience.wav", true, 1.0f, 1.0f, 2.7f, 0.0f);
		
		// wait for 1 second
		Sleep(1000);
		
		// change the volume of a sound while it's playing
		YOA_SetVoiceVolume(voiceID, 0.75f);
		
		// wait for 300 ms
		Sleep(300);
		
		// pauses the entire audio engine
		YOA_Pause();
		Sleep(300);
		
		// resumes the audio engine
		YOA_Resume();
		
		// pan a sound in the stereo field while playing
		YOA_SetVoicePan(voiceID, -0.863);
		Sleep(2500);
		
		// Stop loop using VoiceID
		YOA_StopVoice(voiceID, 0.3f);
		// when stopping a voice with a fade time of 0.0f
		// the audio engine will set a 10ms fade to avoid artifacts

		// quit YoAudio (in this case informing it to also shut down SDL2)
		YOA_Quit(true);
	}

## Build Instructions <a name="build"></a>

if you want a quick and easy sandbox to test stuff in clone the [YoAudio Editor](https://github.com/fuzzblob/YoAudioEditor) repository into a folder adjacent to the YoAudio repository and build. It will find the YoAudio sources, and copy the built *YoAudio.dll* and *YoAudio.pdb* from the YoAudio repo so it can run.

### Dependencies <a name="dependencies"></a>

- An installation of [CMake 3.5 or newer](https://cmake.org/)
- Simple DirectMedia Layer [download SDL Development Libraries 2.x.x (stable)](https://github.com/libsdl-org/SDL/releases)
	- SDL2 is being used as the audio rendering backend (potentially others will be added later)
- **[submodule]** YoAudio is using [sdplog](https://github.com/gabime/spdlog) for printing to the console (not included in Release builds). It is an optional **git submodule**.

**On Submodules:**

YoAudio uses a git feature called submoculed to tie in other git repositories as libraries. If you don't want to or can't use them, you will have to make the dependencies work by hand (adding files to `dependencies/` and possibly editing *CMakeLists.txt*). This can be finicky as the *CMake* scripts require specific folder locations-

To clone the repository with submodules paste the following command in your terminal

> git clone --recurse-submodules https://github.com/fuzzblob/YoAudio.git

Alternatively use a git client that supports git submodules like [Tortoise Git](https://tortoisegit.org/) or [SourceTree](https://www.sourcetreeapp.com/).

- [git documentation](https://git-scm.com/book/en/v2/Git-Tools-Submodules)
- [how to clone submodules](https://stackoverflow.com/questions/3796927/how-to-git-clone-including-submodules))

### Building on Windows <a name="windows"></a>

At the moment the build process has only ever been tested on Windows with Microsoft Visual Studio 2017 Community Edition. Follow these steps to generate a Visual Studio project:

- install Visual Studio (17 2022 recommended) including MSVC compiler and Windows SDK
- clone the repository: `git clone --recurse-submodules https://github.com/fuzzblob/YoAudio.git`
- download the [SDL2 Development Libraries](https://github.com/libsdl-org/SDL/releases/download/release-2.32.2/SDL2-devel-2.32.2-VC.zip) (zip file labeled **"SDL2-devel-2.XX.X-VC"**) and extract the `include` and `lib` folders to `./dependencies/SDL2`
- install [CMAKE](https://cmake.org/download/) (Version 3.5 or newer)
- create & run a build script targeting your tool chain or IDE (an example is provided for Visual Studio 2017 `build_Windows_VisualStudio2017.bat`)
- open the created solution / project file ("YoAudio.sln") from the "build" folder and build the code (`Ctrl + Shift + B`)
- the output will be built to `/bin/DEBUG`
	- YoAudio.dll (runtime)
	- YoAudio.pbd (debug symbols))

### Building on Linux <a name="linux"></a>

- install build essetials (e.g. on Debian run '''apt install build-essential''' bringing in libc-dev, gcc, g++, make)
- install SDL2:
    - Arch: pacman -S sdl2
    - Debian: apt-get install sdl2 or apt-get install libsdl2-2.0-0 or apt-get install libsdl2-dev
    - Fedora: dnf install SDL2-devel
    - Gentoo: emerge media-libs/libsdl2
- clone the repository: `git clone --recurse-submodules https://github.com/fuzzblob/YoAudio.git`
- install CMAKE
- create & run a build script targeting your tool chain or IDE (an example is provided for CodeLite IDE `build_Linux_CodeLite.sh`)
- build
- the output will be built to `./lib/` as **libYoAudio.so**

### Building on Mac OS <a name="macos"></a>

- install SDL via [homebrew](https://brew.sh/) (*CMakeLists.txt* is setup to find it in the default install directory)
- clone the repository: `git clone --recurse-submodules https://github.com/fuzzblob/YoAudio.git`
- run CMake
- build

As I don't have a Mac OS build environment I can not provide detailed instructions on how to build YoAudio for that platform. A pull request with a build script, as well as any changes required to *CMakeLists.txt* and source code would be much appreciated.

## TODO <a name="todo"></a>

These are things that should be explored to move forward. In rough order of importance:

- Resampling
	- possibly high quality resampling when loading sample
	- Voice.GetReSample() pitch algorithm needs filter (currently simple lerp)
- add Audio Graph
	- add SampleGenerator interface
		- generalize audio sources (sample playback, streaming (sample with callback), synthesis)
			- GenerateSamples()
			- SamplesRemaining()
		- AudioEffect
			- limiter for final output
			- Filter (HighPass & LowPass)
	- MixerGroups
- Voice management
	- enforce voice limit
		- virtual state: tracking playback position without mixing
	- get a collection of voices currently playing via Mixer::GetPlayingVoices()
	- stop all voices playing the sound via Mixer::StopSound()
- setup proper CMake build support for Mac OS & Linux
- setup C# bindings and test in Unity
- think about reasonable C++ interface (currently only exposing very simple C interface in YoAudio.h)
- multithreading
	- create MessageQueue
	- AudioRenderer on AudioThread::Update()
		- fill circular buffer of AudioFrames for the callback to consume
- AudioListener
	- 3D math
	- vector based amplitude panning
- improve CMake process
  - use find_package() on all platforms by providing "FindSDL2.cmake" in CMAKE_MODULE_PATH
  - Could not find a package configuration file provided by "SDL2" with any of the following names:
    - SDL2Config.cmake
    - sdl2-config.cmake
  - Add the installation prefix of "SDL2" to CMAKE_PREFIX_PATH or set "SDL2_DIR" to a directory containing one of the above files.

## Acknowledgements<a name="acknowledge"></a>

The initial development was inspired by this [Simple SDL2 Audio example](https://github.com/jakebesworth/Simple-SDL2-Audio) but the direction of the software has since diverged from this very simple approach to playing audio.

A huge thank you to [Jeremy Tammik](https://github.com/jeremytammik) for helping me setup some basics, specifically CMake.
