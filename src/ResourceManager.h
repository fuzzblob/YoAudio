#pragma once

#include <map>

#include "Platform.h"
#include "Voice.h"
#include "Sound.h"

class ResourceManager
{
public:
	std::shared_ptr<Sound> GetSound(const std::string & filename);
	std::shared_ptr<Sound> LoadSound(const std::string & filename);
	bool FreeSound(Sound* sound) noexcept;

	~ResourceManager();
private:
	std::string assetPath;
	std::map<std::string, std::shared_ptr<Sound>> mSounds;
};
