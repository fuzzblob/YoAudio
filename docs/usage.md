# [YoAudio](../README.md)
[![Build Status](https://travis-ci.org/fuzzblob/YoAudio.svg?branch=master)](https://travis-ci.org/fuzzblob/YoAudio)  [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

- [Usage](#usage)
	- [Supported Formats](#formats)
	- [Example Code](#code)

## Using YoAudio <a name="usage"></a>

To use YoaAudio in your project the first step is to call `YOA_Init()` to initialize the audio engine. After that you can call `YOA_PlayWavFile(filename, loop, volume, pitch, fadeIn, pan)` to play WAV files.

The `YOA_PlayWavFile()` method returns a VoiceID which can be used to Stop a playing voice (`YOA_StopVoice(id, fadeOut)`), change it's volume (`YOA_SetVoiceVolume(id, newVolume)`) and panning(`YOA_SetVoicePan(id, newPan)`).

If your application goes out of context and you wish to pause audio rendering use `YOA_Pause()` & `YOA_Resume()`.

Once you wish to shut down the audio engine simply call `YOA_Quit(quitSDL)` passing a bool to signal if YoAudio, or your application will hande the Quitting of the SDL library.

An example implementation of it's functionality is provided by the [Editor](editor.md) sub-project.

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