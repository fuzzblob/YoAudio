#pragma once

#include <memory>
#include <vector>
#include <stack>

#include "AudioDevice.h"
#include "ResourceManager.h"
#include "Voice.h"

class Mixer {
public:
	Mixer() noexcept;
	~Mixer();

	void Pause(const bool pause) noexcept;
	bool IsPaused() noexcept;

	uint16_t PlayWavFile(const std::string & filename, const bool loop = false, const float volume = 1.0f, 
		const float pitch = 1.0f, const float fadeIn = 0.0f, const float pan = 0.0f);
	bool StopVoice(const uint16_t id, const float fadeOut = 0.0f);
	void SetVoiceVolume(const uint16_t id, const float value, const float fade = 0.01f);
	void SetVoicePan(const uint16_t id, const float value);
	// TODO: StopSound (useful for unloading also)
private:
	std::shared_ptr<Voice> GetVoiceAvailable();
	std::shared_ptr<Voice> GetVoiceActive(const uint16_t id);
	
	void FillBuffer();
	static inline void AudioCallback(void * userdata, uint8_t * stream, int len);
private:
	std::unique_ptr<AudioDevice> mDevice = nullptr;
	std::vector<float> mixL;
	std::vector<float> mixR;
	std::vector<std::shared_ptr<Voice>> mPlayingAudio;
	std::stack<std::shared_ptr<Voice>> mAvailableVoices;
	uint16_t mVoiceCount = 0;

	std::unique_ptr<ResourceManager> mResources = nullptr;
};