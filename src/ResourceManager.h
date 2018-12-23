#pragma once

#include <map>

#include "Platform.h"
#include "Voice.h"
#include "Sound.h"

#define MAX_VOICES 0

class ResourceManager
{
private:
	std::map<std::string, std::shared_ptr<Sound>> mSounds;
	std::map<uint16_t, std::shared_ptr<Voice>> mVoices;

	uint16_t lastVoice = 0;

public:
	static std::string GetPath(const std::string & filename);

	std::shared_ptr<Sound> GetSound(const std::string & filename);
	std::shared_ptr<Sound> LoadSound(const std::string & filename);
	bool FreeSound(Sound* sound) noexcept;

	uint16_t GetVoiceCount() noexcept;
	std::shared_ptr<Voice> GetVoice();
	ResourceManager() noexcept;
	~ResourceManager();
};
