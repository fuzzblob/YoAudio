#include "YoManager.h"
#include "Log.h"

#include <algorithm>

std::unique_ptr<YoManager> YoManager::sInstance = nullptr;
bool YoManager::sInitialized = false;

YoManager * YoManager::GetInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = std::make_unique<YoManager>();
	}

	return sInstance.get();
}

void YoManager::Release(const bool quitSDL) noexcept
{
	if(sInstance == nullptr)
		return;
	
	sInstance = nullptr;

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
}

bool YoManager::IsInitialized() noexcept
{
	return sInitialized;
}

uint16_t YoManager::PlayWavFile(const std::string & filename, const bool loop, const float volume, const float pitch, const float fadeIn)
{
	if (m_device == nullptr)
	{
		YOA_CRITICAL("Can't play audio. No Device present!");
		return 0;
	}

	std::shared_ptr<Sound> sound = m_resources->GetSound(filename);
	if (sound == nullptr)
	{
		YOA_CRITICAL("Can't play audio. Could not load sound at path: {0}", filename);
		return 0;
	}

	std::shared_ptr<Voice> voice = m_resources->GetVoice();
	if (voice == nullptr)
	{
		YOA_ERROR("Can't play audio. Could not aquire a Voice!");
		return 0;
	}
	voice->Sound = sound;
	voice->Sound->Spec.userdata = this;

	voice->PlayHead = voice->Sound->Buffer;
	voice->LengthRemaining = voice->Sound->Length;
	voice->Volume = volume;
	if (fadeIn > 0.0f) {
		// ensure current target and value are at 0.0f
		voice->smoothVolume.SetValue(0.0f);
		voice->smoothVolume.SetFadeLength(0);
		voice->smoothVolume.UpdateTarget();
		voice->smoothVolume.GetNext();
		// set fade duration (so the audio callback doesn't snap to the newly set value)
		voice->smoothVolume.SetFadeLength(static_cast<int>(fadeIn * m_device->SpecObtained.freq));
	}
	// set fader target
	voice->smoothVolume.SetValue(1.0f);
	voice->Pitch = pitch;
	voice->IsLooping = loop;

	// avoid duplicate Voices
	if (this->StopVoice(voice->ID) == true)
	{
		YOA_WARN("Had to stop Voice: {0}", voice->ID);
	}
		
	// add voice to playing voices vector
	SDL_LockAudioDevice(m_device->DeviceID);
	this->m_playingAudio.push_back(voice);
	voice->State = ToPlay;
	SDL_UnlockAudioDevice(m_device->DeviceID);

	YOA_INFO("Playing audio: {0} with voiceID: {1}", filename.c_str(), voice->ID);
	return voice->ID;
}

bool YoManager::StopVoice(const uint16_t id, const float fadeOut)
{
	if (m_device == nullptr)
	{
		YOA_CRITICAL("Voice stopping failed! no Device present!");
		return false;
	}

	if (id == 0 || id > m_resources->GetVoiceCount())
	{
		YOA_ERROR("Can't stop specified Voice. Invadid voiceID: {0}", id);
		return false;
	}

	// loop throu sound channels removing stopped voices
	std::vector<std::shared_ptr<Voice>>::iterator it = m_playingAudio.begin();
	std::vector<std::shared_ptr<Voice>>::iterator end = m_playingAudio.end();
	for (; it != end; ++it) {
		auto voice = (*it);
		if (voice->ID != id)
			continue;
		float fade = fadeOut;
		if(fadeOut <= 0.0f)
			fade = 0.01f;
		voice->smoothVolume.SetValue(0.0f);
		voice->smoothVolume.SetFadeLength(static_cast<int>(fade * m_device->SpecObtained.freq));
		
		SDL_LockAudioDevice(m_device->DeviceID);
		voice->State = Stopping;
		//m_playingAudio.erase(it);
		SDL_UnlockAudioDevice(m_device->DeviceID);
		return true;
	}
	return false;
}

void YoManager::Pause(const bool pause) noexcept
{
	if (m_device == nullptr)
	{
		YOA_ERROR("No Device present!");
		return;
	}

	if (pause)
	{
		if (m_Paused)
		{
			YOA_WARN("YoAudio already paused");
			return;
		}
		
		YOA_INFO("YoAudio pausing");
		SDL_PauseAudioDevice(m_device->DeviceID, 1);
		m_Paused = true;
	}
	else
	{
		if (!m_Paused)
		{
			YOA_WARN("YoAudio already playing");
			return;
		}
		
		YOA_INFO("YoAudio resuming");
		SDL_PauseAudioDevice(m_device->DeviceID, 0);
		m_Paused = false;
	}
}

bool YoManager::IsPaused() noexcept
{
	return m_Paused;
}

void YoManager::Run()
{
	YOA_INFO("Thread started!");

	mTimer = std::make_unique<Timer>();
	m_resources = std::make_unique<ResourceManager>();
	m_device = std::make_unique<AudioDevice>();

	SDL_memset(&(m_device->SpecWanted), 0, sizeof(m_device->SpecWanted));

	(m_device->SpecWanted).freq = AUDIO_FREQUENCY;
	(m_device->SpecWanted).format = AUDIO_FORMAT;
	(m_device->SpecWanted).channels = AUDIO_CHANNELS;
	(m_device->SpecWanted).samples = AUDIO_SAMPLES;
	(m_device->SpecWanted).callback = AudioCallback;
	(m_device->SpecWanted).userdata = nullptr;

	if ((m_device->DeviceID = SDL_OpenAudioDevice(nullptr, 0, &(m_device->SpecWanted), &(m_device->SpecObtained), ALLOWED_CHANGES)) == 0)
	{
		YOA_CRITICAL("Warning: failed to open audio device: {0}", SDL_GetError());
		m_Quit = true;
	}
	else
	{
		if (m_device->SpecObtained.freq != AUDIO_FREQUENCY
			|| m_device->SpecObtained.format != AUDIO_FORMAT
			|| m_device->SpecObtained.channels != AUDIO_CHANNELS
			|| m_device->SpecObtained.samples != AUDIO_SAMPLES)
		{
			YOA_ERROR("Failed to open audio device with requested parameters!");
		}

		m_mixStream.reserve(m_device->SpecObtained.channels * m_device->SpecObtained.samples);
		for (size_t i = 0; i < m_mixStream.capacity(); i++) {
			m_mixStream.push_back(0.0f);
		}

		// unpause SDL audio callback
		this->Pause(false);
	}

	// main thread loop
	while (m_Quit == false)
	{
		if (m_ThreadRunning == false)
		{
			continue;
		}

		const double targetFrameLength = 1.0 / FRAME_RATE;
		while (mTimer->DeltaTime() < targetFrameLength) {
			// increment deltaTime
			mTimer->Update();
		}

		// call the non-rendering update
		this->Update(mTimer->DeltaTime());

		mTimer->ResetDeltaTime();
	}

	this->Pause(true);

	// close SDL audio
	SDL_CloseAudioDevice(m_device->DeviceID);

	m_resources = nullptr;
	m_device = nullptr;
}

void YoManager::Update(const double deltaTime) noexcept
{
	// system updates
	//YOA_INFO("Current DeltaTime: {0}", deltaTime);

	if (m_Paused)
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

YoManager::YoManager() noexcept
{
#if !SPDLOG_ENABLED
	printf("YoAudio initializing. (logging disabled). version: %i %i\n\n", YOA_VERSION_MAJOR, YOA_VERSION_MINOR);
#endif //SPDLOG_ENABLED

	// initialize logging
	Log::Init();
	YOA_INFO("YoAudio initializing. version: {0} {1}", YOA_VERSION_MAJOR, YOA_VERSION_MINOR);

	if (SDL_WasInit(SDL_INIT_AUDIO) != 0)
	{
		YOA_WARN("Audio is already initialized.");
	}
	
	// initialize SDL audio
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{

		YOA_WARN("Failed to initilize SDL! {0}", SDL_GetError());
		return;
	}

	YOA_INFO("Yo Audio Init");
	
	if (!(SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO))
	{
		YOA_ERROR("SDL_INIT_AUDIO not initialized");
		return;
	}

	m_Quit = false;
	m_ThreadRunning = true;
	m_Thread = std::thread([this]() { this->Run(); });
	// alterate: std::thread t{&Class::Run, this}

	// TODO: check SpecWanted against SpecObtained
}

YoManager::~YoManager()
{
	// stop the SDL AudioCallback
	this->Pause(true);

	m_Quit = true;
	// wait for thread to finish
	m_Thread.join();

	YOA_INFO("YoAudio Shutdown sucessfully");
}

inline void YoManager::AudioCallback(void * userdata, uint8_t * stream, int len)
{
	// convert from bytes to amount of 16bit samples expected
	const uint32_t streamLen = uint32_t(len / 2);
	// fill float buffer with silence
	std::vector<float> mixBuffer = sInstance->m_mixStream;
	for (size_t i = 0; i < streamLen; i++) {
		mixBuffer[i] = 0.0f;
	}

	// s16 to normalized float
	const float sampleFactor = 1.0f / 32768.0f;
	for (auto voice : sInstance->m_playingAudio)
	{
		if (voice->State == ToPlay)
			voice->State = Playing;
		if (voice->State == Playing || voice->State == Stopping)
		{
			const float volumeFactor = voice->Volume;
			const float pitch = voice->Pitch;

			uint32_t length = ((streamLen > voice->LengthRemaining / 2) ? voice->LengthRemaining / 2 : streamLen);

			const Sint16* samples = (Sint16*)voice->PlayHead;
			float sampleIndex = 0;
			float sample;

			voice->smoothVolume.UpdateTarget();
			for (uint32_t i = 0; i < length; i++)
			{
				sample = samples[static_cast<int>(sampleIndex)] * sampleFactor;
				mixBuffer[i] += sample * volumeFactor * voice->smoothVolume.GetNext();
				// TODO: implement interpolating pitching & resampling
				sampleIndex += pitch; // non-interpolating pitching
			}

			voice->PlayHead += (int)(length * 2 * pitch);
			voice->LengthRemaining -= (int)(length * 2 * pitch);

			if (voice->State == Stopping
				&& voice->smoothVolume.HasReachedTarget())
				voice->State = Stopped;
			if (voice->LengthRemaining <= 0)
			{
				if (voice->IsLooping == true)
				{
					// TODO: make seamless
					voice->PlayHead = voice->Sound->Buffer;
					voice->LengthRemaining = voice->Sound->Length;

					length = streamLen - length;
				}
				else
				{
					// Non looping sound has no more mixable samples
					voice->State = Stopped;
				}
			}
		}
	}
	// fill 16bit output buffer
	Sint16* current = (Sint16*)stream;
	for (uint32_t i = 0; i < streamLen; i++)
	{
		// clipping if float buffer outside of render range
		if (mixBuffer[i] > 1.0f)
			mixBuffer[i] = 1.0f;
		else if (mixBuffer[i] < -1.0f)
			mixBuffer[i] = -1.0f;
		// convert float back to 16bit
		current[i] = static_cast<Sint16>(mixBuffer[i] * 32767);
	}
	// update render time
	sInstance->mTimer->AdvancemRenderTime(streamLen);
}

