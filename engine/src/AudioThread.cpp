#include "AudioThread.h"

#include "EngineConfig.h"
#include "Log.h"
#include "Platform.h"
#include "Timer.h"
// includes <SDL.h>

#include <algorithm>
#include <memory>
#ifndef AUDIO_THREAD_UPDATES
#include <stdlib.h>
#endif
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

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
			using milliseconds = std::chrono::milliseconds;
			if (mThreadRunning == false) {
				continue;
			}
#ifdef AUDIO_THREAD_UPDATES
			// sanity check: make sure that the requested AudioThread update rate is high enough
			constexpr double targetTickLength = 1.0 / TICK_RATE;
			constexpr double calculatedTickLength = 1.0 * TARGET_SAMPLERATE / TARGET_BUFFER;
			static_assert(targetTickLength < calculatedTickLength,
				"AudiThread updates must happen more frequently than the Mixer::AudioCallback");
			// ping the timer until i'ts time to update the engine state
			while (mTimer->DeltaTime() < targetTickLength) {
				const auto delta = mTimer->DeltaTime();
				const auto wait = targetTickLength - delta;
				constexpr auto sleepThreshold = 1.5;
				if(wait >= sleepThreshold)
				{
					const auto sleepLength = static_cast<int>(std::max(floor(wait) - 1, 1.0));
					const auto sleep = milliseconds(sleepLength);
					// wait a little
					std::this_thread::sleep_for(sleep);
				}
				// increment deltaTime
				mTimer->Update();
			}

			if (!mMixer)
			{
				YOA_WARN("AudioThread::Update without active Mixer")
			}
			else if(mMixer->IsPaused() == false)
			{
				mMixer->Update(mTimer->DeltaTime());
			}

			mTimer->ResetDeltaTime();
#else
			// wait for 10 ms before checking if the thread should continue running. used to be "Sleep(10);"
			std::this_thread::sleep_for(milliseconds(WAIT_TIME_MILLI_SECONDS));
#endif
		}
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

#ifdef AUDIO_THREAD_UPDATES
		// initialize timer for deltaTime
		mTimer = std::make_unique<Timer>();
#endif

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
