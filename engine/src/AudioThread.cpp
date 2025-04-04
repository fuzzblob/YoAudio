#include "AudioThread.h"

#include "Log.h"
#include <memory>
#if AUDIO_THREAD_UPDATES == false
#include <stdlib.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#endif

namespace YoaEngine
{
	std::unique_ptr<AudioThread> AudioThread::sInstance = nullptr;
	bool AudioThread::sInitialized = false;

	AudioThread* AudioThread::GetInstance(const bool creatIfNull)
	{
		if (!sInstance) {
			if (!creatIfNull) {
				return nullptr;
			}
			sInstance = std::make_unique<AudioThread>();
		}

		return sInstance.get();
	}

	void AudioThread::Release(const bool quitSDL) noexcept
	{
		if (!sInstance) {
			return;
		}
		// stop audio engine
		sInstance->mMixer = nullptr;

		if (quitSDL == true)
		{
			// quit SDL
			SDL_Quit();
		}
		else
		{
			// only quit SDL Audio system
			SDL_QuitSubSystem(SDL_INIT_AUDIO);
		}
		// remove singleton
		sInstance = nullptr;
	}

	void AudioThread::Run()
	{
		YOA_TRACE("Thread started!");

		// main thread loop
		while (mQuit == false)
		{
			if (mThreadRunning == false) {
				continue;
			}
#if AUDIO_THREAD_UPDATES
			constexpr double targetFrameLength = 1.0 / FRAME_RATE;
			while (mTimer->DeltaTime() < targetFrameLength) {
				// increment deltaTime
				mTimer->Update();
			}

			Update();
			mTimer->ResetDeltaTime();
#else
			// wait for 10 ms before checking if the thread should continue running
			//Sleep(10);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
#endif
		}
	}

	void AudioThread::Update() noexcept
	{
		//YOA_INFO("Current DeltaTime: {0}", mTimer->DeltaTime());

		if (!mMixer || mMixer->IsPaused()) {
			return;
		}
		// interpolate values
		// update statemachines
		// fill ring buffer
	}

	AudioThread::AudioThread()
	{
#if LOGGING_ENABLED && SPDLOG_FOUND
		// initialize logging
		Log::Init();
#endif
		YOA_TRACE("{0} initializing. version: {1}", PROJECT_NAME, PROJECT_VERSION);

		if (SDL_WasInit(SDL_INIT_AUDIO) != 0) {
			YOA_WARN("Audio is already initialized.");
		}
		// initialize SDL audio
		else if (SDL_Init(SDL_INIT_AUDIO) < 0) {
			YOA_CRITICAL("Failed to initilize SDL! {0}", SDL_GetError());
			return;
		}
		// check itialization went well
		if (!(SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO)) {
			YOA_CRITICAL("SDL_INIT_AUDIO not initialized");
			return;
		}

		// initialize Mixer
		mMixer = std::make_shared<Mixer>();
		mMixer->Pause(false);

		mQuit = false;
		mThreadRunning = true;
		sInitialized = true;
		mThread = std::thread([this]() { this->Run(); });
	}

	AudioThread::~AudioThread()
	{
		mQuit = true;
		// wait for thread to finish
		mThread.join();
		AudioThread::sInitialized = false;
	}
}  // namespace YoaEngine
