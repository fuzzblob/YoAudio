#ifndef _YOMANAGER_H
#define _YOMANAGER_H

#include <thread>
#include <atomic>
#include <vector>

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
	static YoManager* sInstance;
	static bool sInitialized;

	static AudioDevice * Device;
	std::vector<float> m_stream;
	std::vector<Voice*> m_playingAudio;
	bool mPaused = true;
	
	std::thread mThread;
	const int UPDATE_RATE = 30; // Updates per second
	std::atomic_bool mQuit = false;
	std::atomic_bool mThreadRunning = true;
	Timer * mTimer = nullptr;
public:
	static YoManager* GetInstance();
	static void Release(bool quitSDL);
	static bool IsInitialized();

	uint16_t PlayWavFile(const std::string & filename, bool loop, float volume, float pitch);
	bool StopVoice(uint16_t id);
	void Pause(bool pause);
	bool IsPaused();
private:
	YoManager();
	~YoManager();

	bool Init();
	void Run();
	void Update();
	
	void QueueVoice(Voice * newVoice);
	static inline void AudioCallback(void * userdata, uint8_t * stream, int len);
};

#endif // !_YOMANAGER_H
