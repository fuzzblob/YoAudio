#pragma once

#include <memory>
#include <atomic>
#include <thread>

#include "Timer.h"
#include "Mixer.h"

namespace YoaEngine
{
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

		std::unique_ptr<AudioThread> sInstance = nullptr;
		bool sInitialized = false;

		std::atomic_bool mQuit;
		std::atomic_bool mThreadRunning;
		std::thread mThread;
	};
};