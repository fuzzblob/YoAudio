![image: fox with headphones](misc/fox_small.png)

# YoAudio

- [About](#about)
- [Overview](#overview)
	- [Usage](#usage)
	- [Dependencies](#dependencies)
- [Build Instructions](#build)
- [TODO](#todo)
- [Acknowledgements](#acknowledge)

## About <a name="about"></a>

YoAudio is a C++ audio environment meant as a playground for experimentation with game audio concepts and DSP algorithms.

The name Yo is based on the phonetic approximation of the korean word for fox (여우).

**WARNING! YoAudio is not release quality!**

## Overview <a name="overview"></a>

### Using YoAudio <a name="usage"></a>

To use YoaAudio in your project the first step is to call `YOA_Init()` to initialize the audio engine. After that you can call `YOA_PlayWavFile(filename, loop, volume, pitch, fadeIn, pan)` to play WAV files. The `YOA_PlayWavFile()` method returns a VoiceID which can be used to Stop a playing voice (`YOA_StopVoice(id, fadeOut)`), change it's volume (`YOA_SetVoiceVolume(id, newVolume)`) and panning(`YOA_SetVoicePan(id, newPan)`). If your application goes out of context and you wish to pause audio rendering use `YOA_Pause()` & `YOA_Resume()`. Once you wish to shut down the audio engine simply call `YOA_Quit(quitSDL)` passing a bool to signal if YoAudio, or your application will hande the Quitting of the SDL library.

Here is the basic idea:

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
		
		// change the volume of a sound while it's playing
		YOA_SetVoiceVolume(voiceID, 0.75f);
		
		// pan a sound in the stereo field while playing
		YOA_SetVoicePan(voiceID, -0.863);

		// Stop loop using VoiceID
		YOA_StopVoice(voiceID, 0.3f);
		// when stopping a voice with a fade time of 0.0f
		// the audio engine will set a 10ms fade to avoid artifacts

		// pauses the entire audio engine
		YOA_Pause();
		// resumes the audio engine
		YOA_Resume();

		// quit YoAudio (in this case informing it to also shut down SDL2)
		YOA_Quit(true);
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

### Linux

Please provide me with information on how to make this work properly. A pull request with a build script (calling CMake), as well as any changes required to *CMakeLists.txt* and source code would be much appreciated.

### Mac OS

Same as Linux.

- you can install SDL via homebrew. *CMakeLists.txt* is setup to find it in the default install directory and **should** pick it up

## TODO <a name="todo"></a>

These are things that should be explored to move forward. In rough order of importance:

- add resampling of WAV data (Sample.Frequency to AudioDevice.Frequency)
	- currently files that' don't match the device samplerate get layed back pitched
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

## Acknowledgements<a name="acknowledge"></a>

The initial development was heavily inspired by this [Simple SDL2 Audio example](https://github.com/jakebesworth/Simple-SDL2-Audio).