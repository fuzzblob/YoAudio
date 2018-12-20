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
		YOA_WARN("Failed to open wave file: {0}\nerror: {1}", filename, SDL_GetError());
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
		YOA_ERROR("Can't stop NULL sound!");
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
		if (voice.second != nullptr && voice.second->State == Stopped)
		{
			newVoice = voice.second;
			break;
		}
	}
	
	// else make a new voice
	if (newVoice == nullptr)
	{
		if(lastVoice >= maxVoices)
		{
			YOA_ERROR("Reached Voice limit of {0}", maxVoices);
			return nullptr;
		}
			
		newVoice = std::make_shared<Voice>();
		if (newVoice == nullptr)
		{
			YOA_ERROR("Memory allocation error while making new Voice");
			return nullptr;
		}

		// set Voice ID
		lastVoice++;
		newVoice->ID = lastVoice;
		mVoices[newVoice->ID] != newVoice;
	}
	
	newVoice->Sound = nullptr;
	return newVoice;
}

ResourceManager::ResourceManager() noexcept
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

	mVoices.clear();
}
