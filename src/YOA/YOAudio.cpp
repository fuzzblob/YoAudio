#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include "Defs.h"
#include "YOAudio.h"
#include "ResourceManager.h"

// 0                                    No changes
// SDL_AUDIO_ALLOW_FREQUENCY_CHANGE     frequency changes (e.g. AUDIO_FREQUENCY = 48000, but allow files to play at 44.100)
// SDL_AUDIO_ALLOW_FORMAT_CHANGE        format change (e.g. AUDIO_FORMAT = S32LSB, but allow wave files of S16LSB to play)
// SDL_AUDIO_ALLOW_CHANNELS_CHANGE      channel number (e.g. AUDIO_CHANNELS = 2, allow actual 1)
// SDL_AUDIO_ALLOW_ANY_CHANGE           all changes above
#define ALLOWED_CHANGES SDL_AUDIO_ALLOW_CHANNELS_CHANGE

#define AUDIO_FORMAT AUDIO_S16LSB
#define AUDIO_FREQUENCY 48000
#define AUDIO_CHANNELS 2
#define AUDIO_SAMPLES 4096

typedef struct AudioDevice
{
	SDL_AudioDeviceID DeviceID;
	SDL_AudioSpec SpecWanted;
	SDL_AudioSpec SpecObtained;
} AudioDevice;

// Voices
static void QueueVoice(Voice * next);
std::vector<float> m_stream;
std::vector<Voice*> m_playingAudio;

// SDL audio callback
static inline void AudioCallback(void * userdata, uint8_t * stream, int len);

// static variables
static AudioDevice * Device = nullptr;

int YOA_Init(void)
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
			return 1;
		}

		// TODO: check if SDL is only used by YoAudio
		// then quit on Realease() like so:
		// SDL_QuitSubSystem(SDL_INIT_AUDIO);
	}

	printf("Yo Audio Init\n");

	Device = (AudioDevice*)malloc(sizeof(AudioDevice));

	if (Device == nullptr)
	{
		fprintf(stderr, "[%s:\t%d]\nFatal Error: Memory c-allocation error\n\n", __FILE__, __LINE__);
		return 1;
	}

	if (!(SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO))
	{
		fprintf(stderr, "[%s:\t%d]\nError: SDL_INIT_AUDIO not initialized\n\n", __FILE__, __LINE__);
		return 1;
	}

	SDL_memset(&(Device->SpecWanted), 0, sizeof(Device->SpecWanted));

	(Device->SpecWanted).freq = AUDIO_FREQUENCY;
	(Device->SpecWanted).format = AUDIO_FORMAT;
	(Device->SpecWanted).channels = AUDIO_CHANNELS;
	(Device->SpecWanted).samples = AUDIO_SAMPLES;
	(Device->SpecWanted).callback = AudioCallback;
	(Device->SpecWanted).userdata = nullptr;

	// want.userdata = newSound;
	if ((Device->DeviceID = SDL_OpenAudioDevice(nullptr, 0, &(Device->SpecWanted), &(Device->SpecObtained), ALLOWED_CHANGES)) == 0)
	{
		fprintf(stderr, "[%s:\t%d]Warning: failed to open audio device: %s\n\n", __FILE__, __LINE__, SDL_GetError());
		return 1;
	}
	else
	{
		// TODO: check SpecWanted against SpecObtained

		// unpause SDL audio callback
		YOA_Resume();
		return 0;
	}
}

void YOA_Quit(void)
{
	printf("Yo Audio Quit\n");
	if(Device == nullptr)
	{
		fprintf(stderr, "[%s:\t%d]\nError: no audio device initialized!\n\n", __FILE__, __LINE__);
		return;
	}

	YOA_Pause();

	ResourceManager::GetInstance()->FreeSound((Sound *)(Device->SpecWanted).userdata);

	// close SDL audio
	SDL_CloseAudioDevice(Device->DeviceID);

	free(Device);
	Device = nullptr;

	ResourceManager::Release();
}

uint16_t PlayWavFile(const char * filename, bool loop, int volume)
{
	printf("Playing: %s\n", filename);

	if (Device == nullptr)
	{
		fprintf(stderr, "[%s:\t%d]\nError: no audio device initialized!\n\n", __FILE__, __LINE__);
		return 0;
	}

	Voice * newVoice = ResourceManager::GetInstance()->GetVoice(std::string(filename), loop, volume);

	if (newVoice != nullptr)
	{
		SDL_LockAudioDevice(Device->DeviceID);
		QueueVoice(newVoice);
		SDL_UnlockAudioDevice(Device->DeviceID);

		printf("%s got ID: %i\n", filename, newVoice->ID);
		return newVoice->ID;
	}
	else
	{
		fprintf(stderr, "[%s:\t%d]\nError: Can't queue NULL voice!\n\n", __FILE__, __LINE__);
		return 0;
	}
}

int StopVoice(uint16_t id)
{
	if (id == 0 || id > ResourceManager::GetInstance()->GetVoiceCount())
	{
		printf("Invadid ID: %i\nCan't stop specified Voice as it does not exist!\n\n", id);
		return 1;
	}

	printf("Stopping Voice: %i\n", id);

	std::vector<Voice*>::iterator it = m_playingAudio.begin();
	std::vector<Voice*>::iterator end = m_playingAudio.end();

	for (; it != end; ++it)
	{
		if ((*it._Ptr)->ID == id)
		{
			SDL_LockAudioDevice(Device->DeviceID);

			m_playingAudio.erase(it);

			SDL_UnlockAudioDevice(Device->DeviceID);
			return 0;
		}
	}

	return 1;
}

void YOA_Pause(void)
{
	printf("Yo Audio Pause\n");

    if(Device != nullptr)
    {
        SDL_PauseAudioDevice(Device->DeviceID, 1);
    }
}

void YOA_Resume(void)
{
	printf("Yo Audio Resume\n");

    if(Device != nullptr)
    {
        SDL_PauseAudioDevice(Device->DeviceID, 0);
    }
}

void QueueVoice(Voice * newVoice)
{
	// avoid duplicate Voices
	if (StopVoice(newVoice->ID) == 0)
	{
		fprintf(stderr, "[%s:\t%d]\nError: had to stop ID %i before it could be played!\n\n", __FILE__, __LINE__, newVoice->ID);
	}

	SDL_LockAudioDevice(Device->DeviceID);
	
	m_playingAudio.push_back(newVoice);

	SDL_UnlockAudioDevice(Device->DeviceID);
}

static inline void AudioCallback(void * userdata, uint8_t * stream, int len)
{
	int streamLen = len / 2;
	m_stream.reserve(streamLen);
	float* floatStream = m_stream.begin()._Ptr;

	for (int i = 0; i < streamLen; i++)
	{
		floatStream[i] = 0.0f;
	}

	std::vector<Voice*>::iterator it = m_playingAudio.begin();
	std::vector<Voice*>::iterator end = m_playingAudio.end();

	for (; it != end; ++it)
	{
		Voice * voice = *it._Ptr;

		if (voice->State == ToPlay)
		{
			voice->State = Playing;
		}

		if (voice->State == Playing)
		{
			// s16 to normalized float
			float sampleFactor = 1 / 32768.0f;
			float volumeFactor = voice->Volume;
			float pitch = voice->Pitch;

			/*
			int processedSamples = 0;

			while (processedSamples < streamLen)
			{
				// TODO: implement seamless looping
			}
			*/

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

	Sint16* current = (Sint16*) stream;
	for (int i = 0; i < streamLen; i++)
	{
		float val = floatStream[i];

		// clipping
		if (val > 1.0f)
		{
			val = 1.0f;
		}
		else if (val < -1.0f)
		{
			val = -1.0f;
		}

		// convert float back to s16
		current[i] = (Sint16)(val * 32767);
	}
}
