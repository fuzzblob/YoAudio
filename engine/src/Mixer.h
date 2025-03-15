#pragma once

#include <memory>
#include <vector>
#include <stack>

#include "AudioDevice.h"
#include "ResourceManager.h"
#include "Voice.h"

namespace YoaEngine
{
	class Mixer {
	public:
		Mixer();
		~Mixer();

		void Pause(const bool pause) noexcept;
		bool IsPaused() noexcept;

		uint32_t PlayWavFile(const std::string& filename, const bool loop = false, const float volume = 1.0f,
			const float pitch = 1.0f, const float fadeIn = 0.0f, const float pan = 0.0f);
		bool StopVoice(const uint32_t id, const float fadeOut = 0.0f);
		void SetVoiceVolume(const uint32_t id, const float value, const float fade = 0.01f);
		void SetVoicePan(const uint32_t id, const float value);
		void StopSound(const float fade = 0.01f);
	private:
		std::shared_ptr<Voice> GetVoiceAvailable();
		std::shared_ptr<Voice> GetVoiceActive(const uint32_t id);

		void FillBuffer();
		static inline void AudioCallback(void* userdata, uint8_t* stream, int len);
	private:
		std::unique_ptr<AudioDevice> mDevice = nullptr;
		std::vector<float> mixL;
		std::vector<float> mixR;
		std::vector<std::shared_ptr<Voice>> mPlayingAudio;
		std::stack<std::shared_ptr<Voice>> mAvailableVoices;
		uint32_t mVoiceCount = 0u;

		std::unique_ptr<ResourceManager> mResources = nullptr;
	};
}