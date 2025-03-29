#include "ResourceManager.h"

#include "../include/Platform.h"
#include "Log.h"
#include "AudioDevice.h"

namespace YoaEngine
{
	std::shared_ptr<Sample> ResourceManager::GetSound(const std::string& filename)
	{
		if (assetPath.size() == 0) {
			auto sdl_path = SDL_GetBasePath();
			assetPath = std::string(sdl_path);

			// TODO: change slash dependent on platform
#if YOA_PLATFORM == YOA_UNIX || YOA_PLATFORM == YOA_MAC
			assetPath.append("assets/");
#elif YOA_PLATFORM == YOA_WINDOWS
			assetPath.append("assets\\");
#else
#error audio backend for this platform currently not implemented
#endif

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

	std::shared_ptr<Sample> ResourceManager::LoadSound(const std::string& filePath)
	{
		std::shared_ptr<Sample> newSound = std::make_shared<Sample>();
		SDL_AudioSpec spec;
		uint32_t audioLength;
		if (SDL_LoadWAV(filePath.c_str(), &spec, &newSound->Buffer, &audioLength) == nullptr) {
			YOA_ERROR("Failed to open wave file: {0}\nerror: {1}", filePath, SDL_GetError());
			return nullptr;
		}

		newSound->Channels = spec.channels;
		newSound->Frequency = spec.freq;
		newSound->Format = AudioDevice::GetYoaFormat(spec);

		size_t bytesPerSample = 0;
		switch (newSound->Format)
		{
		case YOA_Format_Uint8:
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
			YOA_ERROR("Unknown sample format! - {0} / {1}", filePath, SDL_AUDIO_BITSIZE(spec.format));
			break;
		}
		newSound->Samples = (audioLength) / (bytesPerSample * newSound->Channels);
		YOA_TRACE("Loaded sound at \n\t{0}\n\tYOA_FORMAT: {1} - sample data: {2}bit {3} kHz", filePath,
			(unsigned char)(newSound->Format), (bytesPerSample * 8), newSound->Frequency);

		return newSound;
	}

	void ResourceManager::FreeSound(std::shared_ptr<Sample> sound) noexcept {
		if (sound == nullptr) {
			YOA_ERROR("Can't stop NULL sound!");
			return;
		}
		// free buffer if not null
		if (sound->Buffer != nullptr)
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
}
