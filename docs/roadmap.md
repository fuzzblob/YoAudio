# [YoAudio](../README.md)
[![Build Status](https://travis-ci.org/fuzzblob/YoAudio.svg?branch=master)](https://travis-ci.org/fuzzblob/YoAudio)  [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

- [TODO](#todo)

## TODO <a name="todo"></a>

These are things that should be explored to move forward. In rough order of importance:

- Add other rendering backends than SDL2
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