#pragma once

#include <memory>
#include <map>

#include "Sample.h"

class ResourceManager {
public:
	std::shared_ptr<Sample> GetSound(const std::string & filename);

	~ResourceManager();
private:
	std::shared_ptr<Sample> LoadSound(const std::string & filename);
	void FreeSound(std::shared_ptr<Sample> sound) noexcept;
private:
	std::string assetPath;
	std::map<std::string, std::shared_ptr<Sample>> mSounds;
};
