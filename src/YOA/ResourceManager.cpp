#include "ResourceManager.h"

ResourceManager* ResourceManager::sInstance = nullptr;

ResourceManager * ResourceManager::GetInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new ResourceManager();
	}

	return sInstance;
}

void ResourceManager::Release()
{
	if (sInstance == nullptr)
		return;

	delete sInstance;
	sInstance = nullptr;
}

std::string ResourceManager::GetPath(std::string sFilename)
{
	std::string fullPath = std::string(SDL_GetBasePath());
	fullPath.append("assets\/" + sFilename);
	return fullPath;
}

Sound * ResourceManager::GetSound(const char * cFilename)
{
	std::string fullPath = GetPath(cFilename);

	if (mSounds[fullPath] == nullptr)
	{
		mSounds[fullPath] = LoadSound(fullPath.c_str());
	}

	return mSounds[fullPath];
}

Sound * ResourceManager::LoadSound(const char * cFilename)
{
	Sound * newSound = (Sound*)malloc(sizeof(Sound));

	if (newSound == nullptr)
	{
		fprintf(stderr, "[%s:\t%d]\nError: Memory allocation error\n\n", __FILE__, __LINE__);
		return nullptr;
	}

	if (SDL_LoadWAV(cFilename, &(newSound->Spec), &(newSound->Buffer), &(newSound->SampleLength)) == nullptr)
	{
		fprintf(stderr, "[%s:\t%d]\nWarning: failed to open wave file: %s error: %s\n\n", __FILE__, __LINE__, cFilename, SDL_GetError());
		free(newSound);
		return nullptr;
	}

	newSound->Buffer = newSound->Buffer;
	newSound->Length = newSound->SampleLength;
	(newSound->Spec).callback = nullptr;
	(newSound->Spec).userdata = nullptr;

	return newSound;
}

bool ResourceManager::FreeSound(Sound* sound)
{
	SDL_FreeWAV(sound->Buffer);

	return false;
}

uint16_t ResourceManager::GetVoiceCount()
{
	return lastVoice;
}

Voice * ResourceManager::GetVoice(const char * filename, bool loop, int volume)
{
	Voice * newVoice = nullptr;

	// Try getting a voice for recycling
	for (auto voice : mVoices)
	{
		if (voice.second->State == Stopped)
		{
			voice.second->State = ToPlay;
			newVoice = voice.second;
		}
	}
	
	// else make a new voice
	if (newVoice == nullptr)
	{
		newVoice = (Voice*)malloc(sizeof(Voice));

		if (newVoice == nullptr)
		{
			fprintf(stderr, "[%s:\t%d]\nError: Memory allocation error\n\n", __FILE__, __LINE__);
			return nullptr;
		}
	}
	
	newVoice->Sound = sInstance->GetSound(filename);

	if (newVoice->Sound == nullptr)
	{
		free(newVoice);
		return nullptr;
	}

	lastVoice++;

	newVoice->ID = lastVoice;
	newVoice->State = ToPlay;
	newVoice->PlayHead = newVoice->Sound->Buffer;
	newVoice->LengthRemaining = newVoice->Sound->Length;
	newVoice->Next = nullptr;
	newVoice->Volume = volume;
	newVoice->IsLooping = loop;

	if (mVoices[lastVoice] != nullptr)
	{
		fprintf(stderr, "[%s:\t%d]\nError: New Voice could not be stored in voice map!\n\n", __FILE__, __LINE__);
	}
	else
	{
		mVoices[lastVoice] = newVoice;
	}

	return newVoice;
}

ResourceManager::ResourceManager()
{
	lastVoice = 0;
}

ResourceManager::~ResourceManager()
{
	for (auto snd : mSounds)
	{
		if (snd.second != nullptr)
		{
			SDL_FreeWAV(snd.second->Buffer);
		}
	}
	mSounds.clear();

	for (auto voice : mVoices)
	{
		if (voice.second != nullptr)
		{
			free(voice.second);
		}
	}
	mSounds.clear();
	
}
