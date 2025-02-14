#include "AudioThread.h"

#include "Log.h"
#if AUDIO_THREAD_UPDATES == false
#include <stdlib.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#endif

std::unique_ptr<AudioThread> AudioThread::sInstance = nullptr;
bool AudioThread::sInitialized = false;

AudioThread* AudioThread::GetInstance(const bool creatIfNull)
{
	if (sInstance == nullptr) {
		if (!creatIfNull)
			return nullptr;
		sInstance = std::make_unique<AudioThread>();
	}

	return sInstance.get();
}

void AudioThread::Release(const bool quitSDL) noexcept
{
	if (sInstance == nullptr)
		return;
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
#if LOGGING_ENABLED
	// initialize logging
	Log::Init();
#endif
	YOA_TRACE("YoAudio initializing. version: {0} {1}", YOA_VERSION_MAJOR, YOA_VERSION_MINOR);

	if (SDL_WasInit(SDL_INIT_AUDIO) != 0){
		YOA_WARN("Audio is already initialized.");
	}
	// initialize SDL audio
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		YOA_CRITICAL("Failed to initilize SDL! {0}", SDL_GetError());
		return;
	}
	// check itialization went well
	if (!(SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO)) {
		YOA_CRITICAL("SDL_INIT_AUDIO not initialized");
		return;
	}

	mTimer = std::make_unique<Timer>();

	// initialize Mixer
	mMixer = std::make_unique<Mixer>();
	mMixer->Pause(false);

	YOA_TRACE("Thread started!");

	// main thread loop
	while (mQuit == false) 
	{
		if (mThreadRunning == false)
			continue;
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
		std::this_thread::sleep_for (std::chrono::milliseconds(10));
#endif
	}
}

void AudioThread::Update() noexcept
{
	//YOA_INFO("Current DeltaTime: {0}", mTimer->DeltaTime());

	if (mMixer == nullptr || mMixer->IsPaused())
		return;

	// interpolate values
	// update statemachines
	// fill ring buffer
}

AudioThread::AudioThread()
{
	mQuit = false;
	mThreadRunning = true;
	mThread = std::thread([this]() { this->Run(); });
}

AudioThread::~AudioThread()
{
	mQuit = true;
	// wait for thread to finish
	mThread.join();
}
