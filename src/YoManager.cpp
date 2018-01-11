#include "YoManager.h"

#include "Defs.h"

YoManager * YoManager::sInstance = nullptr;
bool YoManager::sInitialized = false;
AudioDevice * YoManager::Device = nullptr;

YoManager * YoManager::GetInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new YoManager();
	}

	return sInstance;
}

void YoManager::Release(bool quitSDL)
{
	delete sInstance;
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

bool YoManager::IsInitialized()
{
	return sInitialized;
}

uint16_t YoManager::PlayWavFile(const std::string & filename, bool loop, float volume, float pitch)
{
	printf("Playing: %s\n", filename.c_str());

	if (Device == nullptr)
	{
		fprintf(stderr, "[%s:\t%d]\nError: no audio device initialized!\n\n", __FILE__, __LINE__);
		return 0;
	}

	Voice * newVoice = ResourceManager::GetInstance()->GetVoice();
	newVoice->Sound = ResourceManager::GetInstance()->GetSound(filename);

	if (newVoice->Sound == nullptr)
	{
		newVoice->State = Stopped;
		return 0;
	}

	newVoice->PlayHead = newVoice->Sound->Buffer;
	newVoice->LengthRemaining = newVoice->Sound->Length;
	newVoice->Volume = volume;
	newVoice->Pitch = pitch;
	newVoice->IsLooping = loop;

	if (newVoice != nullptr)
	{
		sInstance->QueueVoice(newVoice);

		printf("%s got ID: %i\n", filename.c_str(), newVoice->ID);
		return newVoice->ID;
	}
	else
	{
		fprintf(stderr, "[%s:\t%d]\nError: Can't queue NULL voice!\n\n", __FILE__, __LINE__);
		return 0;
	}
}

bool YoManager::StopVoice(uint16_t id)
{
	if (id == 0 || id > ResourceManager::GetInstance()->GetVoiceCount())
	{
		printf("Invadid ID: %i\nCan't stop specified Voice as it does not exist!\n\n", id);
		return false;
	}

	std::vector<Voice*>::iterator it = m_playingAudio.begin();
	std::vector<Voice*>::iterator end = m_playingAudio.end();

	for (; it != end; ++it)
	{
		if ((*it._Ptr)->ID == id)
		{
			SDL_LockAudioDevice(Device->DeviceID);
			m_playingAudio.erase(it);
			SDL_UnlockAudioDevice(Device->DeviceID);

			if ((*it._Ptr)->State != Stopped)
			{
				printf("Stopped Voice: %i\n", id);
				(*it._Ptr)->State = Stopped;
				return true;
			}
		}
	}

	return false;
}

void YoManager::Pause(bool pause)
{
	if (Device == nullptr)
	{
		printf("NULL Error: no Device present!");
		return;
	}

	if(pause == true)
	{
		if (mPaused == true)
		{
			printf("Yo Audio already paused\n");
			// already paused
		}
		else
		{
			printf("Yo Audio pausing\n");
			SDL_PauseAudioDevice(Device->DeviceID, 1);
			mThreadRunning = false;
			mPaused = true;
		}
	}
	else
	{
		if (mPaused == false)
		{
			printf("Yo Audio already playing\n");
			// already playing
		}
		else
		{
			printf("Yo Audio resuming\n");
			mThreadRunning = true;
			SDL_PauseAudioDevice(Device->DeviceID, 0);
			mPaused = false;
		}
	}
}

bool YoManager::IsPaused()
{
	return mPaused;
}

void YoManager::Run()
{
	printf("Thread started!\n");

	while (mQuit == false)
	{
		if (mThreadRunning == false)
		{
			continue;
		}

		mTimer->Update();
		if (mTimer->DeltaTime() >= 1.0f / UPDATE_RATE)
		{
			//printf("DeltaTime: %f\n", mTimer->DeltaTime());
			this->Update();
			mTimer->Reset();
		}
	}

	printf("Thread finished!\n");
}

void YoManager::Update()
{
	//printf("updates!\n");
}

YoManager::YoManager()
{
	sInitialized = Init();
}

YoManager::~YoManager()
{
	// stop the SDL AudioCallback
	this->Pause(true);

	mThreadRunning = false;
	mQuit = true;
	// wait for thread to finish
	mThread.join();

	printf("Yo Audio Quit\n");
	if (Device == nullptr)
	{
		fprintf(stderr, "[%s:\t%d]\nError: no audio device initialized!\n\n", __FILE__, __LINE__);
		return;
	}

	// close SDL audio
	SDL_CloseAudioDevice(Device->DeviceID);

	free(Device);
	Device = nullptr;

	ResourceManager::Release();
}

bool YoManager::Init()
{
	if (SDL_WasInit(SDL_INIT_AUDIO) != 0)
	{
		printf("Audio is already initialized.\n");
	}
	else
	{
		// initialize SDL audio
		if (SDL_Init(SDL_INIT_AUDIO) < 0)
		{
			fprintf(stderr, "[%s:\t%d]Warning: failed to initilize SDL!\n\n", __FILE__, __LINE__);
			return false;
		}
	}

	printf("Yo Audio Init\n");

	mTimer = new Timer();

	Device = (AudioDevice*)malloc(sizeof(AudioDevice));

	if (Device == nullptr)
	{
		fprintf(stderr, "[%s:\t%d]\nFatal Error: Memory c-allocation error\n\n", __FILE__, __LINE__);
		return false;
	}

	if (!(SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO))
	{
		fprintf(stderr, "[%s:\t%d]\nError: SDL_INIT_AUDIO not initialized\n\n", __FILE__, __LINE__);
		return false;
	}

	SDL_memset(&(Device->SpecWanted), 0, sizeof(Device->SpecWanted));

	(Device->SpecWanted).freq = AUDIO_FREQUENCY;
	(Device->SpecWanted).format = AUDIO_FORMAT;
	(Device->SpecWanted).channels = AUDIO_CHANNELS;
	(Device->SpecWanted).samples = AUDIO_SAMPLES;
	(Device->SpecWanted).callback = AudioCallback;
	(Device->SpecWanted).userdata = nullptr;

	if ((Device->DeviceID = SDL_OpenAudioDevice(nullptr, 0, &(Device->SpecWanted), &(Device->SpecObtained), ALLOWED_CHANGES)) == 0)
	{
		fprintf(stderr, "[%s:\t%d]Warning: failed to open audio device: %s\n\n", __FILE__, __LINE__, SDL_GetError());
		return false;
	}
	else
	{
		mQuit = false;
		mThreadRunning = true;
		mThread = std::thread([this]() { this->Run(); });
		// alterate: std::thread t{&Class::Run, this}

		// TODO: check SpecWanted against SpecObtained

		// unpause SDL audio callback
		this->Pause(false);

		return true;
	}
}

void YoManager::QueueVoice(Voice * newVoice)
{
	// avoid duplicate Voices
	sInstance->StopVoice(newVoice->ID); // returns true because we probably set the state to ToPlay
	// set voice state
	newVoice->State = ToPlay;

	SDL_LockAudioDevice(Device->DeviceID);
	// add voice to playing voices vector
	sInstance->m_playingAudio.push_back(newVoice);
	SDL_UnlockAudioDevice(Device->DeviceID);
}

inline void YoManager::AudioCallback(void * userdata, uint8_t * stream, int len)
{
	int streamLen = len / 2;
	
	std::vector<float>* fstream = &(sInstance->m_stream);
	
	fstream->reserve(streamLen);
	float* floatStream = fstream->begin()._Ptr;

	for (int i = 0; i < streamLen; i++)
	{
		floatStream[i] = 0.0f;
	}

	std::vector<Voice*>::iterator it = sInstance->m_playingAudio.begin();
	std::vector<Voice*>::iterator end = sInstance->m_playingAudio.end();

	for (; it != end; ++it)
	{
		Voice * voice = *it._Ptr;

		if (voice->State == ToPlay)
		{
			voice->State = Playing;
		}

		if (voice->State == Playing)
		{
			// TODO: make dependent on sound.spec.format
			// s16 to normalized float
			float sampleFactor = 1 / 32768.0f;
			float volumeFactor = voice->Volume;
			float pitch = voice->Pitch;

			uint32_t length = (uint32_t)((streamLen > voice->LengthRemaining / 2) ? voice->LengthRemaining / 2 : streamLen);

			Sint16* samples = (Sint16*)voice->PlayHead;
			float sampleIndex = 0;

			// TODO: pitch sounds according to tehir samplerate vs device rate
			for (uint32_t i = 0; i < length; i++)
			{
				if (i == length - 1)
				{
					voice = voice;
				}

				floatStream[i] = (samples[(int)sampleIndex] * 1.0f) * sampleFactor * volumeFactor;

				// TODO: implement interpolating pitching & resampling
				// non-interpolating pitching
				sampleIndex += pitch;
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
	for (int i = 0; i < streamLen; i++)
	{
		float val = floatStream[i];

		// TODO: implement a lookahead brickwall limiter
		// clipping
		if (val > 1.0f)
		{
			val = 1.0f;
		}
		else if (val < -1.0f)
		{
			val = -1.0f;
		}

		// TODO: make dependent on device.spec.format
		// convert float back to s16
		current[i] = (Sint16)(val * 32767);
	}
}
