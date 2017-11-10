#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

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
static Voice * QueueVoice(Voice * next);

// SDL audio callback
static inline void AudioCallback(void * userdata, uint8_t * stream, int len);

// static variables
static AudioDevice * Device = nullptr;
static Voice* rootVoice;

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
	(Device->SpecWanted).userdata = calloc(1, sizeof(Sound));

	// TODO: Chris, let's think about this one a bit
	rootVoice = (Voice*)(Device->SpecWanted).userdata;
	if (rootVoice == nullptr)
	{
		fprintf(stderr, "[%s:\t%d]Error: Memory allocation error\n\n", __FILE__, __LINE__);
		return 1;
	}

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

uint16_t PlaySound(const char * filename, bool loop, int volume)
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

void StopVoice(uint16_t id)
{
	if (rootVoice == nullptr)
	{
		return;
	}

	if (id == 0 || id > ResourceManager::GetInstance()->GetVoiceCount())
	{
		printf("Invadid ID: %i\nCan't stop specified Voice as it does not exist!\n\n", id);
		return;
	}

	printf("Stopping Voice: %i\n", id);

	// get Voice matching the ID
	Voice * current = rootVoice;
	
	while (current->Next != nullptr)
	{
		if (current->ID == id)
		{
			break;
		}

		current = current->Next;
	}

	if(current->State != Stopped)
		current->State = Stopping;

	return;
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

Voice * QueueVoice(Voice * newVoice)
{
	// TODO: use Vector insread of linked list
	if (rootVoice == nullptr)
	{
		return nullptr;
	}

	// get the last voice in the linked list
	Voice * last = rootVoice;
	while (last->Next != nullptr)
	{
		last = last->Next;
	}

	last->Next = newVoice;

	return nullptr;
}

static inline void AudioCallback(void * userdata, uint8_t * stream, int len)
{
	Voice * voice = (Voice *) userdata;
	Voice * previous = voice;
	uint32_t tempLength;

    // fill buffer with silence
    SDL_memset(stream, 0, len);

    // first one is place holder
    voice = voice->Next;

    while(voice != nullptr)
    {
		if (voice->State == Stopping)
		{
			voice->State = Stopped;

			// remove stopping voice from linked list
			previous->Next = voice->Next;
			voice = previous->Next;
		}
		else
		{
			// TODO: seamless looping by iterating over samples -> sample remaining

			tempLength = ((uint32_t)len > voice->LengthRemaining) ? voice->LengthRemaining : (uint32_t)len;
			SDL_MixAudioFormat(stream, voice->PlayHead, AUDIO_FORMAT, tempLength, voice->Volume);

			voice->PlayHead += tempLength;
			voice->LengthRemaining -= tempLength;

			if (voice->LengthRemaining <= 0)
			{
				if (voice->IsLooping == true)
				{
					voice->PlayHead = voice->Sound->Buffer;
					voice->LengthRemaining = voice->Sound->Length;
				}
				else
				{
					// Non looping sound has no more mixable samples
					voice->State = Stopped;

					// remove stopping voice from linked list
					if (voice->Next != nullptr)
					{
						previous->Next = voice->Next;
						// prepare for progressing through the linked list
						voice = previous;
					}
				}
			}

			previous = voice;
			voice = previous->Next;
		}
    }
}
