#pragma once

#include "YoaConfig.h"
#include "Platform.h"
#include "Log.h"

// 0                                    No changes
// SDL_AUDIO_ALLOW_FREQUENCY_CHANGE     frequency changes (e.g. AUDIO_FREQUENCY = 48000, but allow files to play at 44.100)
// SDL_AUDIO_ALLOW_FORMAT_CHANGE        format change (e.g. AUDIO_FORMAT = S32LSB, but allow wave files of S16LSB to play)
// SDL_AUDIO_ALLOW_CHANNELS_CHANGE      channel number (e.g. AUDIO_CHANNELS = 2, allow actual 1)
// SDL_AUDIO_ALLOW_ANY_CHANGE           all changes above
#define ALLOWED_CHANGES 0

#if TARGET_BITDEPTH == 8
#define AUDIO_FORMAT AUDIO_S8
#elif TARGET_BITDEPTH == 16
#define AUDIO_FORMAT AUDIO_S16SYS
#elif TARGET_BITDEPTH == 32
#define AUDIO_FORMAT AUDIO_S32SYS
#else
#error unsupported target bitdepth
#endif;

class AudioDevice {
private:
	SDL_AudioDeviceID DeviceID;
	// is the audio device paused
	bool mPaused = true;
public:
	// audio callback buffer size (per channel)
	uint32_t Samples = 0;
	// audio callback channel count
	uint8_t Channels = 0;
	// audio callback sample rate
	uint32_t Frequency = 0;
	// audio callback bit depth
	SampleFormat Format = YOA_Format_Unknown;

	
	bool IsPaused() const noexcept {
		return mPaused;
	}
	void SetPaused(const bool pause) noexcept {
		if (mPaused == pause)
			return;
		YOA_INFO("audio engine state set to \"paused = {0}\"", pause);
		mPaused = pause;
		SDL_PauseAudioDevice(DeviceID, pause ? 1 : 0);
	}

	void Lock() {
		SDL_LockAudioDevice(DeviceID);
	}
	void Unlock() {
		SDL_UnlockAudioDevice(DeviceID);
	}

	const char* GetDeviceName() const {
		return SDL_GetCurrentAudioDriver();
	}

	static SampleFormat ConvertFormat(const SDL_AudioSpec& spec) {
		if (SDL_AUDIO_ISFLOAT(spec.format)) {
			return YOA_Format_Float;
		}
		else {
			switch (SDL_AUDIO_BITSIZE(spec.format)) {
			case 8:
				return YOA_Format_Sint8;
			case 16:
				return YOA_Format_Sint16;
			case 32:
				return YOA_Format_Sint32;
			default:
				return YOA_Format_Unknown;
			}
		}
	}

	AudioDevice(void* userData, void(*callback)(void* userdata, uint8_t* stream, int len)) {
		SDL_AudioSpec want;
		SDL_memset(&want, 0, sizeof(want));
		want.freq = TARGET_SAMPLERATE;
		want.format = AUDIO_FORMAT;
		want.channels = TARGET_CHANNELS;
		want.samples = TARGET_BUFFER;
		want.callback = callback;
		want.userdata = userData;
		SDL_AudioSpec get;

		DeviceID = SDL_OpenAudioDevice(nullptr, 0, &want, &get, ALLOWED_CHANGES);
		if (DeviceID == 0) {
			YOA_CRITICAL("Failed to open audio device: {0}", SDL_GetError());
			return;
		}
		else if (get.freq != TARGET_SAMPLERATE
			|| get.format != AUDIO_FORMAT
			|| get.channels != TARGET_CHANNELS
			|| get.samples != TARGET_BUFFER)
		{
			YOA_ERROR("AudioDevice opened with different parameters than requested!");
		}
		SetPaused(true);

		Samples = get.samples;
		Channels = get.channels;
		Frequency = get.freq;
		Format = AudioDevice::ConvertFormat(get);

		YOA_INFO("Opened AudioDevice \"{0}\" ID: {1}\n\t{2} sample rate, {5}bit, {3} channels, buffer size {4}",
			GetDeviceName(), DeviceID, Frequency, Channels, Samples, Format);
	}
	~AudioDevice() {
		SetPaused(true);
		// close SDL audio
		SDL_CloseAudioDevice(DeviceID);
	}

};
