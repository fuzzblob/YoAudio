#pragma once

#include <memory>
#include <vector>

#include "AudioDevice.h"
#include "ResourceManager.h"
#include "Voice.h"

class Mixer {
public:
	Mixer();
	~Mixer();

	void Pause(const bool pause) noexcept;
	bool IsPaused() noexcept;

	uint16_t PlayWavFile(const std::string & filename, const bool loop = false, const float volume = 1.0f, const float pitch = 1.0f, const float fadeIn = 0.0f);
	bool StopVoice(const uint16_t id, const float fadeOut = 0.0f);
private:
	void FillBuffer();
	static inline void AudioCallback(void * userdata, uint8_t * stream, int len);
private:
	bool m_Paused = true;
	std::unique_ptr<AudioDevice> mDevice = nullptr;
	std::vector<float> mMixStream;
	std::vector<std::shared_ptr<Voice>> mPlayingAudio;
	std::unique_ptr<ResourceManager> mResources = nullptr;
};