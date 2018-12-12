#ifndef _YOMANAGER_H
#define _YOMANAGER_H

#include <thread>
#include <atomic>
#include <vector>
#include <memory>

#include "AudioDevice.h"
#include "Timer.h"
#include "ResourceManager.h"

// 0                                    No changes
// SDL_AUDIO_ALLOW_FREQUENCY_CHANGE     frequency changes (e.g. AUDIO_FREQUENCY = 48000, but allow files to play at 44.100)
// SDL_AUDIO_ALLOW_FORMAT_CHANGE        format change (e.g. AUDIO_FORMAT = S32LSB, but allow wave files of S16LSB to play)
// SDL_AUDIO_ALLOW_CHANNELS_CHANGE      channel number (e.g. AUDIO_CHANNELS = 2, allow actual 1)
// SDL_AUDIO_ALLOW_ANY_CHANGE           all changes above
#define ALLOWED_CHANGES SDL_AUDIO_ALLOW_CHANNELS_CHANGE

#define AUDIO_FORMAT AUDIO_S16LSB
#define AUDIO_FREQUENCY 48000
#define AUDIO_CHANNELS 2
#define AUDIO_SAMPLES 4096

class YoManager
{
private:
	const int UPDATE_RATE = 30; // Updates per second
	
	static std::unique_ptr<YoManager> sInstance;
	static bool sInitialized;

	bool m_Paused = true;
	std::unique_ptr<AudioDevice> m_device = nullptr;
	std::vector<float> m_stream;
	std::vector<std::shared_ptr<Voice>> m_playingAudio;
	std::unique_ptr<ResourceManager> m_resources = nullptr;
	
	std::thread m_Thread;
	std::atomic_bool m_Quit = false;
	std::atomic_bool m_ThreadRunning = true;
public:
	static YoManager* GetInstance();
	static void Release(bool quitSDL);
	static bool IsInitialized();

	YoManager() noexcept;
	~YoManager() noexcept;
	
	uint16_t PlayWavFile(const std::string & filename, bool loop, float volume, float pitch);
	bool StopVoice(uint16_t id) noexcept;
	void Pause(bool pause) noexcept;
	bool IsPaused() noexcept;
private:
	void Run();
	void Update() noexcept;
	
	static inline void AudioCallback(void * userdata, uint8_t * stream, int len);
};

#endif // !_YOMANAGER_H
