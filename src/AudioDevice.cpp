#include "AudioDevice.h"

#include "Log.h"

bool AudioDevice::IsPaused() const noexcept {
	return mPaused;
}
void AudioDevice::SetPaused(const bool pause) {
	if (mPaused == pause)
		return;
	YOA_INFO("audio engine state set to \"paused = {0}\"", pause);
	mPaused = pause;
	SDL_PauseAudioDevice(DeviceID, pause ? 1 : 0);
}

void AudioDevice::Lock() noexcept {
	SDL_LockAudioDevice(DeviceID);
}
void AudioDevice::Unlock() noexcept {
	SDL_UnlockAudioDevice(DeviceID);
}

const char* AudioDevice::GetDeviceName() const noexcept {
	return SDL_GetCurrentAudioDriver();
}

SampleFormat AudioDevice::ConvertFormat(const SDL_AudioSpec& spec) noexcept {
	if (SDL_AUDIO_ISFLOAT(spec.format)) {
		return YOA_Format_Float;
	}
	else {
		switch (SDL_AUDIO_BITSIZE(spec.format)) {
		case 8:
			if (SDL_AUDIO_ISSIGNED(spec.format))
				return YOA_Format_Sint8;
			else
				return YOA_Format_Uint8;
		case 16:
			return YOA_Format_Sint16;
		case 32:
			return YOA_Format_Sint32;
		default:
			return YOA_Format_Unknown;
		}
	}
}

AudioDevice::AudioDevice(void* userData, void(*callback)(void* userdata, uint8_t* stream, int len)) {
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
AudioDevice::~AudioDevice() {
	SetPaused(true);
	// close SDL audio
	SDL_CloseAudioDevice(DeviceID);
}