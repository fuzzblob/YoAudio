#pragma once

#include <memory>
#include <atomic>
#include <thread>

#include "Timer.h"
#include "Mixer.h"

class AudioThread {
public:
	static AudioThread* GetInstance(const bool creatIfNull = false);
	static void Release(const bool quitSDL) noexcept;


	AudioThread();
	~AudioThread();

	std::unique_ptr<Timer> mTimer = nullptr;
	std::unique_ptr<Mixer> mMixer = nullptr;
private:
	void Run();
	void Update() noexcept;
private:
	static std::unique_ptr<AudioThread> sInstance;
	static bool sInitialized;

	std::atomic_bool mQuit = false;
	std::atomic_bool mThreadRunning = true;
	std::thread mThread;
};