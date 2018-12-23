#ifndef _YOMANAGER_H
#define _YOMANAGER_H

#include <thread>
#include <atomic>
#include <vector>
#include <memory>

#include "AudioDevice.h"
#include "Timer.h"
#include "ResourceManager.h"

class YoManager
{
private:
	static std::unique_ptr<YoManager> sInstance;
	static bool sInitialized;

	bool m_Paused = true;
	std::unique_ptr<AudioDevice> m_device = nullptr;
	std::vector<float> m_mixStream;
	std::vector<std::shared_ptr<Voice>> m_playingAudio;
	std::unique_ptr<ResourceManager> m_resources = nullptr;
	
	std::unique_ptr<Timer> mTimer = nullptr;
	std::thread m_Thread;
	std::atomic_bool m_Quit = false;
	std::atomic_bool m_ThreadRunning = true;
public:
	static YoManager* GetInstance();
	static void Release(const bool quitSDL) noexcept;
	static bool IsInitialized() noexcept;

	YoManager() noexcept;
	~YoManager();
	
	uint16_t PlayWavFile(const std::string & filename, const bool loop = false, const float volume = 1.0f, const float pitch = 1.0f, const float fadeIn = 0.0f);
	bool StopVoice(const uint16_t id, const float fadeOut = 0.0f);
	void Pause(const bool pause) noexcept;
	bool IsPaused() noexcept;
private:
	void Run();
	void Update() noexcept;
	
	static inline void AudioCallback(void * userdata, uint8_t * stream, int len);
};

#endif // !_YOMANAGER_H
