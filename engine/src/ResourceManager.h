#pragma once

#include "Sample.h"

#include <memory>
#include <map>
#include <string>

namespace YoaEngine
{
	class ResourceManager {
	public:
		ResourceManager(const ResourceManager &) = default;
		ResourceManager(ResourceManager &&) = delete;
		ResourceManager &operator=(const ResourceManager &) = default;
		ResourceManager &operator=(ResourceManager &&) = delete;
		explicit ResourceManager(
			std::string assetPath = "",
			std::map<std::string, std::shared_ptr<Sample>> soundDict =
				std::map<std::string, std::shared_ptr<Sample>>());
		~ResourceManager();

		std::shared_ptr<Sample> GetSound(const std::string &filename);
	private:
		std::shared_ptr<Sample> LoadSound(const std::string& filePath);
		void FreeSound(const std::shared_ptr<Sample> sound) noexcept;
	private:
		std::string mAssetPath;
		std::map<std::string, std::shared_ptr<Sample>> mSounds;
	};
}  // namespace YoaEngine
