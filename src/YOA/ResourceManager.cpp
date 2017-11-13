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

std::string ResourceManager::GetPath(const std::string & filename)
{
	std::string fullPath = std::string(SDL_GetBasePath());
	fullPath.append("assets/" + filename);
	return fullPath;
}

Sound * ResourceManager::GetSound(const std::string & filename)
{
	std::string fullPath = GetPath(filename);

	if (mSounds[fullPath] == nullptr)
	{
		mSounds[fullPath] = LoadSound(fullPath);
	}

	return mSounds[fullPath];
}

Sound * ResourceManager::LoadSound(const std::string & filename)
{
	Sound * newSound = (Sound*)malloc(sizeof(Sound));

	if (newSound == nullptr)
	{
		fprintf(stderr, "[%s:\t%d]\nError: Memory allocation error\n\n", __FILE__, __LINE__);
		return nullptr;
	}

	if (SDL_LoadWAV(filename.c_str(), &(newSound->Spec), &(newSound->Buffer), &(newSound->Length)) == nullptr)
	{
		fprintf(stderr, "[%s:\t%d]\nWarning: failed to open wave file: %s error: %s\n\n", __FILE__, __LINE__, filename, SDL_GetError());
		free(newSound);
		return nullptr;
	}

	(newSound->Spec).callback = nullptr;
	(newSound->Spec).userdata = nullptr;

	return newSound;
}

bool ResourceManager::FreeSound(Sound* sound)
{
	if(sound->Buffer != nullptr)
		SDL_FreeWAV(sound->Buffer);
	
	sound->Buffer = nullptr;

	return false;
}

uint16_t ResourceManager::GetVoiceCount()
{
	return lastVoice;
}

Voice * ResourceManager::GetVoice(const std::string & filename, bool loop, int volume)
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

		// set Voice ID
		lastVoice++;
		newVoice->ID = lastVoice;
	}
	
	newVoice->Sound = sInstance->GetSound(filename);

	if (newVoice->Sound == nullptr)
	{
		free(newVoice);
		return nullptr;
	}

	newVoice->State = ToPlay;
	newVoice->PlayHead = newVoice->Sound->Buffer;
	newVoice->LengthRemaining = newVoice->Sound->Length;
	newVoice->Next = nullptr;
	newVoice->Volume = (float) volume / 128.0f;
	newVoice->Pitch = 1.0f;
	newVoice->IsLooping = loop;

	if (mVoices[newVoice->ID] != nullptr && mVoices[newVoice->ID] != newVoice)
	{
		fprintf(stderr, "[%s:\t%d]\nError: New Voice could not be stored in voice map!\n\n", __FILE__, __LINE__);
	}
	else
	{
		mVoices[newVoice->ID] = newVoice;
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
