#include "YoManager.h"
#include "Defs.h"
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

void YoManager::Release(bool quitSDL) noexcept
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

uint16_t YoManager::PlayWavFile(const std::string & filename, bool loop, float volume, float pitch)
{
	YOA_INFO("Playing: {0}", filename.c_str());

	if (m_device == nullptr)
	{
		YOA_ERROR("No Device present!");
		return 0;
	}

	std::shared_ptr<Sound> sound = m_resources->GetSound(filename);
	if (sound == nullptr)
	{
		YOA_ERROR("Could not load sound at path: {0}", filename);
		return 0;
	}

	std::shared_ptr<Voice> newVoice = m_resources->GetVoice();
	if (newVoice == nullptr)
	{
		YOA_ERROR("Could not load sound at path: {0}", filename);
		return 0;
	}
	newVoice->Sound = sound;
	newVoice->Sound->Spec.userdata = this;

	newVoice->PlayHead = newVoice->Sound->Buffer;
	newVoice->LengthRemaining = newVoice->Sound->Length;
	newVoice->Volume = volume;
	newVoice->Pitch = pitch;
	newVoice->IsLooping = loop;

	{
		// avoid duplicate Voices
		if (this->StopVoice(newVoice->ID) == true)
		{
			YOA_WARN("Had to stop Voice: {0}", newVoice->ID);
		}
		
		// add voice to playing voices vector
		SDL_LockAudioDevice(m_device->DeviceID);
		this->m_playingAudio.push_back(newVoice);
		newVoice->State = ToPlay;
		SDL_UnlockAudioDevice(m_device->DeviceID);
	}

	YOA_INFO("{0} got ID: {1}", filename.c_str(), newVoice->ID);
	return newVoice->ID;
}

bool YoManager::StopVoice(uint16_t id)
{
	if (m_device == nullptr)
	{
		YOA_ERROR("NULL Error: no Device present!");
		return false;
	}

	if (id == 0 || id > m_resources->GetVoiceCount())
	{
		YOA_ERROR("Invadid ID: ", id, " Can't stop specified Voice as it does not exist!");
		return false;
	}

	// loop throu sound channels removing stopped voices
	std::vector<std::shared_ptr<Voice>>::iterator it = m_playingAudio.begin();
	std::vector<std::shared_ptr<Voice>>::iterator end = m_playingAudio.end();
	for (; it != end; ++it) {
		auto voice = (*it);
		if (voice->ID != id)
			continue;
		SDL_LockAudioDevice(m_device->DeviceID);
		voice->State = Stopped;
		m_playingAudio.erase(it);
		SDL_UnlockAudioDevice(m_device->DeviceID);
		return true;
	}
	return false;
}

void YoManager::Pause(bool pause) noexcept
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
			YOA_WARN("Yo Audio already paused");
			return;
		}
		
		YOA_INFO("Yo Audio pausing");
		SDL_PauseAudioDevice(m_device->DeviceID, 1);
		m_Paused = true;
	}
	else
	{
		if (!m_Paused)
		{
			YOA_WARN("Yo Audio already playing");
			return;
		}
		
		YOA_INFO("Yo Audio resuming");
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

	std::unique_ptr<Timer> time = std::make_unique<Timer>();
	m_resources = std::make_unique<ResourceManager>();
	m_device = std::make_unique<AudioDevice>();

	SDL_memset(&(m_device->SpecWanted), 0, sizeof(m_device->SpecWanted));

	(m_device->SpecWanted).freq = AUDIO_FREQUENCY;
	(m_device->SpecWanted).format = AUDIO_FORMAT;
	(m_device->SpecWanted).channels = AUDIO_CHANNELS;
	(m_device->SpecWanted).samples = AUDIO_SAMPLES;
	(m_device->SpecWanted).callback = AudioCallback;
	(m_device->SpecWanted).userdata = this;

	if ((m_device->DeviceID = SDL_OpenAudioDevice(nullptr, 0, &(m_device->SpecWanted), &(m_device->SpecObtained), ALLOWED_CHANGES)) == 0)
	{
		YOA_WARN("Warning: failed to open audio device: {0}", SDL_GetError());
		m_Quit = true;
	}
	else
	{
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

		// increment deltaTime
		time->Update();
		if (time->DeltaTime() >= 1.0f / UPDATE_RATE)
		{
			this->Update();
			time->Reset();
		}
	}

	this->Pause(true);

	// close SDL audio
	SDL_CloseAudioDevice(m_device->DeviceID);

	m_resources = nullptr;
	m_device = nullptr;
}

void YoManager::Update() noexcept
{
	// system updates

	if (m_Paused)
		return;

	// interpolate values
	// update statemachines
}

YoManager::YoManager() noexcept
{
	if (SDL_WasInit(SDL_INIT_AUDIO) != 0)
	{
		YOA_WARN("Audio is already initialized.");
	}
	
	// initialize logging
	Log::Init();
	
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

	YOA_INFO("Yo Audio Shutdown sucessfully");
}

inline void YoManager::AudioCallback(void * userdata, uint8_t * stream, int len)
{
	YoManager* instance = static_cast<YoManager*>(userdata);
	const uint32_t streamLen = uint32_t(len / 2);
	
	std::vector<float>* fstream = &(instance->m_stream);
	
	fstream->reserve(streamLen);
	float* floatStream = fstream->begin()._Ptr;

	for (float f : *fstream)
	{
		f = 0.0f;
	}

	// s16 to normalized float
	const float sampleFactor = 1.0f / 32768.0f;
	for (auto voice : instance->m_playingAudio)
	{
		if (voice->State == Stopped
			|| voice->State == Paused)
		{
			continue;
		}
		else if (voice->State == ToPlay)
		{
			voice->State = Playing;
		}

		if (voice->State == Playing)
		{
			const float volumeFactor = voice->Volume;
			const float pitch = voice->Pitch;

			uint32_t length = ((streamLen > voice->LengthRemaining / 2) ? voice->LengthRemaining / 2 : streamLen);

			const Sint16* samples = (Sint16*)voice->PlayHead;
			float sampleIndex = 0;

			for (uint32_t i = 0; i < length; i++)
			{
				if (i == length - 1)
				{
					voice = voice;
				}

				// TODO: sample mixing by adding values together
				floatStream[i] = (samples[static_cast<int>(sampleIndex)] * 1.0f) * sampleFactor * volumeFactor;

				// TODO: implement interpolating pitching & resampling
				sampleIndex += pitch; // non-interpolating pitching
			}

			voice->PlayHead += length * 2;
			voice->LengthRemaining -= length * 2;

			if (voice->LengthRemaining <= 0)
			{
				if (voice->IsLooping == true)
				{
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

	Sint16* current = (Sint16*)stream;
	for (uint32_t i = 0; i < streamLen; i++)
	{
		float val = floatStream[i];
		// clipping
		if (val > 1.0f)
			val = 1.0f;
		else if (val < -1.0f)
			val = -1.0f;

		// convert float back to s16
		current[i] = static_cast<Sint16>(val * 32767);
	}
}
