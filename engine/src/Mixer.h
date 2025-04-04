#pragma once

#include "AudioDevice.h"
#include "Timer.h"
#include "ResourceManager.h"
#include "Voice.h"

#include <memory>
#include <vector>
#include <stack>

namespace YoaEngine
{
	class Mixer {
	public:
		Mixer();
		~Mixer();

		void Pause(const bool pause);
		bool IsPaused() noexcept;
		/// <summary>
		/// Play a sound file
		/// </summary>
		/// <param name="filename">path to the file, get's loaded if needed</param>
		/// <param name="loop">set to loop (needs to be stopped again via ID)</param>
		/// <param name="volume">amplitude factor</param>
		/// <param name="pitch">playback rate factor</param>
		/// <param name="fadeIn">amount in seconds to reach specified volume</param>
		/// <param name="pan">panning (left/right)</param>
		/// <param name="startPaused">Simply prepare the sound but don't play it yet?</param>
		/// <returns>the ID with which to control the sound once it's started</returns>
		uint32_t PlayWavFile(const std::string& filename, const bool loop = false, const float volume = 1.0f,
			const float pitch = 1.0f, const float fadeIn = 0.0f, const float pan = 0.0f, const bool startPaused = false);
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
		std::unique_ptr<Timer> mTimer = nullptr;
		std::unique_ptr<AudioDevice> mDevice = nullptr;
		std::vector<float> mixL;
		std::vector<float> mixR;
		std::vector<std::shared_ptr<Voice>> mPlayingAudio;
		std::stack<std::shared_ptr<Voice>> mAvailableVoices;
		uint32_t mVoiceCount = 0u;

		std::unique_ptr<ResourceManager> mResources = nullptr;
	};
}  // namespace YoaEngine
