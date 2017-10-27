#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "Defs.h"
#include "YOAudio.h"
#include "AssetManager.h"

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

#pragma region PROTOTYPES | STATICS
// Voices
static Voice * QueVoice(Voice * next);
static Voice * GetVoice(const char * filename, bool loop, int volume);

// SDL audio callback
static inline void AudioCallback(void * userdata, uint8_t * stream, int len);

// static variables
static AudioDevice * Device = nullptr;
static Voice* rootVoice;
static uint16_t lastVoice = 0;
#pragma endregion

void YOA_Init(void)
{
	printf("Yo Audio Init\n");

	Device = (AudioDevice*)malloc(sizeof(AudioDevice));

	if (Device == nullptr)
	{
		fprintf(stderr, "[%s: %d]Fatal Error: Memory c-allocation error\n", __FILE__, __LINE__);
		return;
	}

	Device->IsEnabled = false;

	if (!(SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO))
	{
		fprintf(stderr, "[%s:\t%d]\nError: SDL_INIT_AUDIO not initialized\n", __FILE__, __LINE__);
		return;
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
		fprintf(stderr, "[%s: %d]Error: Memory allocation error\n", __FILE__, __LINE__);
		return;
	}

	// want.userdata = newSound;
	if ((Device->DeviceID = SDL_OpenAudioDevice(nullptr, 0, &(Device->SpecWanted), &(Device->SpecObtained), ALLOWED_CHANGES)) == 0)
	{
		fprintf(stderr, "[%s: %d]Warning: failed to open audio device: %s\n", __FILE__, __LINE__, SDL_GetError());
	}
	else
	{
		// TODO: check SpecWanted against SpecObtained

		// enabled device
		Device->IsEnabled = true;

		// unpause SDL audio callback
		YOA_Resume();
	}
}

void YOA_Quit(void)
{
	printf("Yo Audio Quit\n");
	if(Device == nullptr)
	{
		fprintf(stderr, "[%s:\t%d]\nError: no audio device initialized!\n", __FILE__, __LINE__);
		return;
	}

	if (Device->IsEnabled)
	{
		YOA_Pause();

		AssetManager::GetInstance()->FreeSound((Sound *)(Device->SpecWanted).userdata);

		// close SDL audio
		SDL_CloseAudioDevice(Device->DeviceID);
	}

	free(Device);
	Device = nullptr;

	AssetManager::Release();
}

uint16_t PlaySound(const char * filename, bool loop, int volume)
{
	printf("Playing: %s\n", filename);

	if (Device == nullptr)
	{
		fprintf(stderr, "[%s:\t%d]\nError: no audio device initialized!\n", __FILE__, __LINE__);
		return 0;
	}

    Voice * newVoice;

    if(!Device->IsEnabled)
    {
        return 0;
    }

    newVoice = GetVoice(filename, loop, volume);

    SDL_LockAudioDevice(Device->DeviceID);
    QueVoice(newVoice);
    SDL_UnlockAudioDevice(Device->DeviceID);

	printf("%s got ID: %i\n", filename, newVoice->ID);

	return newVoice->ID;
}

void StopVoice(uint16_t id)
{
	printf("Stopping Voice: %i\n", id);

	if (rootVoice == nullptr)
	{
		return;
	}

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

	//previous->next = current->next;
	current->IsStopping = true;

	return;
}

void YOA_Pause(void)
{
	printf("Yo Audio Pause\n");

    if(Device != nullptr 
		&& Device->IsEnabled == true)
    {
        SDL_PauseAudioDevice(Device->DeviceID, 1);
    }
}

void YOA_Resume(void)
{
	printf("Yo Audio Resume\n");

    if(Device != nullptr 
		&& Device->IsEnabled)
    {
        SDL_PauseAudioDevice(Device->DeviceID, 0);
    }
}

Voice * GetVoice(const char * filename, bool loop, int volume)
{
	Voice * newVoice = (Voice*)malloc(sizeof(Voice));

	if (newVoice == nullptr)
	{
		fprintf(stderr, "[%s: %d]Error: Memory allocation error\n", __FILE__, __LINE__);
		return nullptr;
	}

	newVoice->Sound = AssetManager::GetInstance()->GetSound(filename);

	if (newVoice->Sound == nullptr)
	{
		//fprintf(stderr, "[%s: %d]Warning: failed to open wave file: %s error: %s\n", __FILE__, __LINE__, filename, SDL_GetError());
		free(newVoice);
		return nullptr;
	}

	lastVoice++;

	newVoice->ID = lastVoice;
	newVoice->PlayHead = newVoice->Sound->Buffer;
	newVoice->LengthRemaining = newVoice->Sound->Length;
	newVoice->Next = nullptr;
	newVoice->Volume = volume;
	newVoice->IsLooping = loop;
	newVoice->IsStopping = false;

	return newVoice;
}

Voice * QueVoice(Voice * newVoice)
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
    int tempLength;

	// TODO: get a float buffer to mix in
	// - fill with zeros
	// - resample to Device->SpecObtained.freq
	// - convert from voice->Sound->Spec.format to float
	// - convert float buffer to Device->SpecObtained.format

    // fill buffer with silence
    SDL_memset(stream, 0, len);

    // first one is place holder
    voice = voice->Next;

    while(voice != nullptr)
    {
        if(voice->LengthRemaining > 0)
        {
            if(voice->IsStopping == true)
            {
				voice = previous->Next = voice->Next;
				continue;
            }

            if(voice->LengthRemaining == 0)
            {
                tempLength = 0;
            }
            else
            {
                tempLength = ((uint32_t) len > voice->LengthRemaining) ? voice->LengthRemaining : (uint32_t) len;
				SDL_MixAudioFormat(stream, voice->PlayHead, AUDIO_FORMAT, tempLength, voice->Volume);
            }

            voice->PlayHead += tempLength;
            voice->LengthRemaining -= tempLength;

            previous = voice;
            voice = voice->Next;
        }
        else if(voice->IsLooping == true && voice->IsStopping == false)
        {
            voice->PlayHead = voice->Sound->Buffer;
            voice->LengthRemaining = voice->Sound->Length;
        }
        else
        {
            previous->Next = voice->Next;
            SDL_FreeWAV(voice->PlayHead);
            free(voice);

            voice = previous->Next;
        }
    }
}
