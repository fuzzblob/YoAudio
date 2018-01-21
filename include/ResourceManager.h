#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H

#include <map>

#include "Defs.h"
#include "Voice.h"
#include "Sound.h"

class ResourceManager
{
private:
	static ResourceManager* sInstance;

	std::map<std::string, Sound*> mSounds;
	std::map<uint16_t, Voice*> mVoices;

	uint16_t lastVoice = 0;

public:
	static ResourceManager* GetInstance();
	static void Release();
	static std::string GetPath(const std::string & filename);

	Sound* GetSound(const std::string & filename);
	Sound * LoadSound(const std::string & filename);
	bool FreeSound(Sound* sound);

	uint16_t GetVoiceCount();
	Voice * GetVoice();
private:
	ResourceManager();
	~ResourceManager();
};

#endif // !_RESOURCEMANAGER_H
