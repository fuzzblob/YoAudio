#pragma once

#include <memory>
#include <map>
#include <string>

#include "Sample.h"

namespace YoaEngine
{
	class ResourceManager {
	public:
		std::shared_ptr<Sample> GetSound(const std::string& filename);

		~ResourceManager();
	private:
		std::shared_ptr<Sample> LoadSound(const std::string& filePath);
		void FreeSound(std::shared_ptr<Sample> sound) noexcept;
	private:
		std::string assetPath;
		std::map<std::string, std::shared_ptr<Sample>> mSounds;
	};
}  // namespace YoaEngine
