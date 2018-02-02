#include "ResourceManager.h"

std::string ResourceManager::GetPath(const std::string & filename)
{
	std::string fullPath = std::string(SDL_GetBasePath());
	fullPath.append("assets/" + filename);
	return fullPath;
}

std::shared_ptr<Sound >ResourceManager::GetSound(const std::string & filename)
{
	std::string fullPath = GetPath(filename);

	if (mSounds[fullPath] == nullptr)
	{
		mSounds[fullPath] = LoadSound(fullPath);
	}

	return mSounds[fullPath];
}

std::shared_ptr<Sound> ResourceManager::LoadSound(const std::string & filename)
{
	std::shared_ptr<Sound> newSound = std::make_shared<Sound>();

	if (SDL_LoadWAV(filename.c_str(), &(newSound->Spec), &(newSound->Buffer), &(newSound->Length)) == nullptr)
	{
		fprintf(stderr, "[%s:\t%d]\nWarning: failed to open wave file: %s error: %s\n\n", __FILE__, __LINE__, filename, SDL_GetError());
		return nullptr;
	}

	(newSound->Spec).callback = nullptr;
	(newSound->Spec).userdata = nullptr;

	return newSound;
}

bool ResourceManager::FreeSound(Sound* sound) noexcept
{
	if (sound == nullptr)
	{
		printf("Can't stop NULL sound!");
		return false;
	}

	if(sound->Buffer != nullptr)
		SDL_FreeWAV(sound->Buffer);
	
	sound->Buffer = nullptr;

	return false;
}

uint16_t ResourceManager::GetVoiceCount() noexcept
{
	return lastVoice;
}

std::shared_ptr<Voice> ResourceManager::GetVoice()
{
	std::shared_ptr<Voice> newVoice = nullptr;

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
		newVoice = std::make_shared<Voice>();

		if (newVoice == nullptr)
		{
			fprintf(stderr, "[%s:\t%d]\nError: Memory allocation error\n\n", __FILE__, __LINE__);
			return nullptr;
		}

		// set Voice ID
		lastVoice++;
		newVoice->ID = lastVoice;
	}
	
	newVoice->Sound = nullptr;

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

ResourceManager::ResourceManager() noexcept
{
	lastVoice = 0;
}

ResourceManager::~ResourceManager() noexcept
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
			voice.second = nullptr; // .~shared_ptr()
		}
	}
	mSounds.clear();
}
