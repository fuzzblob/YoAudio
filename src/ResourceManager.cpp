#include "ResourceManager.h"
#include "Log.h"
#include "AudioDevice.h"

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
	SDL_AudioSpec spec;
	uint32_t audioLength;
	if (SDL_LoadWAV(filename.c_str(), &spec, &newSound->Buffer, &audioLength) == nullptr) {
		YOA_ERROR("Failed to open wave file: {0}\nerror: {1}", filename, SDL_GetError());
		return nullptr;
	}

	newSound->Channels = spec.channels;
	newSound->Frequency = spec.freq;
	newSound->Format = AudioDevice::ConvertFormat(spec);

	size_t bytesPerSample = 0;
	switch (newSound->Format)
	{
	case YOA_Format_Float:
		bytesPerSample = sizeof(float);
		break;
	case YOA_Format_Sint8:
		bytesPerSample = sizeof(AUDIO_S8);
		break;
	case YOA_Format_Sint16:
		bytesPerSample = sizeof(AUDIO_S16);
		break;
	case YOA_Format_Sint32:
		bytesPerSample = sizeof(AUDIO_S32);
		break;
	default:
		YOA_ERROR("Unknown sample format! - {0} / {1}", filename, SDL_AUDIO_BITSIZE(spec.format));
		break;
	}
	newSound->Samples = (audioLength) / (bytesPerSample * newSound->Channels);

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

ResourceManager::ResourceManager() noexcept
{

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
}
