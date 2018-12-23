#include "AudioThread.h"

#include "Log.h"

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
#if !SPDLOG_ENABLED
	printf("YoAudio initializing. (logging disabled). version: %i %i\n\n", YOA_VERSION_MAJOR, YOA_VERSION_MINOR);
#endif //SPDLOG_ENABLED

	// initialize logging
	Log::Init();
	YOA_INFO("YoAudio initializing. version: {0} {1}", YOA_VERSION_MAJOR, YOA_VERSION_MINOR);

	if (SDL_WasInit(SDL_INIT_AUDIO) != 0)
		YOA_WARN("Audio is already initialized.");
	// initialize SDL audio
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		YOA_WARN("Failed to initilize SDL! {0}", SDL_GetError());
		return;
	}
	// check itialization went well
	if (!(SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO)) {
		YOA_ERROR("SDL_INIT_AUDIO not initialized");
		return;
	}

	mTimer = std::make_unique<Timer>();

	// initialize Mixer
	mMixer = std::unique_ptr<Mixer>(new Mixer());
	mMixer->Pause(false);

	YOA_INFO("Thread started!");

	// main thread loop
	while (mQuit == false) 
	{
		if (mThreadRunning == false)
			continue;

		constexpr double targetFrameLength = 1.0 / FRAME_RATE;
		while (mTimer->DeltaTime() < targetFrameLength) {
			// increment deltaTime
			mTimer->Update();
		}

		Update();
		mTimer->ResetDeltaTime();
	}
}

void AudioThread::Update() noexcept
{
	// system updates
	//YOA_INFO("Current DeltaTime: {0}", mTimer->DeltaTime());

	if (mMixer == nullptr || mMixer->IsPaused())
		return;

	// loop throu sound channels removing stopped voices
	/*std::vector<std::shared_ptr<Voice>>::iterator it = m_playingAudio.begin();
	std::vector<std::shared_ptr<Voice>>::iterator end = m_playingAudio.end();
	for (int i = 0; it + i != end;) {
		auto voice = (*it);
		if (voice->State != Stopped) {
			i++;
			continue;
		}
		SDL_LockAudioDevice(m_device->DeviceID);
		m_playingAudio.erase(it + i);
		SDL_UnlockAudioDevice(m_device->DeviceID);
		end = m_playingAudio.end();
	}*/

	// interpolate values
	// update statemachines
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
