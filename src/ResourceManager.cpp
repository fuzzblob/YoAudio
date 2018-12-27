#include "ResourceManager.h"

#include "Platform.h"
#include "Log.h"
#include "AudioDevice.h"

std::shared_ptr<Sample> ResourceManager::GetSound(const std::string & filename)
{
	if (assetPath.size() == 0) {
		auto sdl_path = SDL_GetBasePath();
		assetPath = std::string(sdl_path);
		assetPath.append("assets/");
		SDL_free(sdl_path);
	}
	// return if file is loaded
	if (mSounds[filename] != nullptr)
		return mSounds[filename];
	// load file if needed
	std::string fullPath = assetPath;
	fullPath.append(filename);
	return mSounds[filename] = LoadSound(fullPath);
}

std::shared_ptr<Sample> ResourceManager::LoadSound(const std::string & filename)
{
	std::shared_ptr<Sample> newSound = std::make_shared<Sample>();
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
	case YOA_Format_Uint8:
	case YOA_Format_Sint8:
		bytesPerSample = 1;
		break;
	case YOA_Format_Sint16:
		bytesPerSample = 2;
		break;
	case YOA_Format_Float:
	case YOA_Format_Sint32:
		bytesPerSample = 4;
		break;
	default:
		YOA_ERROR("Unknown sample format! - {0} / {1}", filename, SDL_AUDIO_BITSIZE(spec.format));
		break;
	}
	newSound->Samples = (audioLength) / (bytesPerSample * newSound->Channels);
	YOA_INFO("Loaded {0}: format {1} - {2}bit {3} kHz", filename, 
		newSound->Format, (bytesPerSample * 8), newSound->Frequency);

	return newSound;
}

void ResourceManager::FreeSound(std::shared_ptr<Sample> sound) noexcept {
	if (sound == nullptr) {
		YOA_ERROR("Can't stop NULL sound!");
		return;
	}
	// free buffer if not null
	if(sound->Buffer != nullptr)
		SDL_FreeWAV(sound->Buffer);
	// ensure buffer points to null
	sound->Buffer = nullptr;
}

ResourceManager::~ResourceManager() {
	for (auto soundEntry : mSounds) {
		FreeSound(soundEntry.second);
	}
	mSounds.clear();
}
