#pragma once

#include "Mixer.h"

#include <memory>
#include <atomic>
#include <thread>

namespace YoaEngine
{
	class AudioThread {
	public:
		static AudioThread* GetInstance(const bool creatIfNull = false);
		static void Release(const bool quitSDL) noexcept;

		AudioThread();
		AudioThread(const AudioThread &) = delete;
		AudioThread(AudioThread &&) = delete;
		AudioThread &operator=(const AudioThread &) = delete;
		AudioThread &operator=(AudioThread &&) = delete;
		~AudioThread();

		std::shared_ptr<Mixer> GetMixer() const { return mMixer; };
	private:
		void Run();
		void Update() noexcept;
	private:
		static std::unique_ptr<AudioThread> sInstance;
		std::shared_ptr<Mixer> mMixer = nullptr;
		static bool sInitialized;

		std::atomic_bool mQuit;
		std::atomic_bool mThreadRunning;
		std::thread mThread;
	};
}  // namespace YoaEngine
