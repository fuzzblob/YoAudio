#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H

#include <map>

#include "Defs.h"

typedef struct Sound
{
	uint32_t Length;
	uint8_t * Buffer;

	SDL_AudioSpec Spec;
} Sound;

enum VoiceState
{
	ToPlay = 1,
	Playing = 2,
	Paused = 3,
	Stopping = 4,
	Stopped  = 5
};

typedef struct Voice
{
	uint16_t ID;
	VoiceState State;
	Sound * Sound;
	uint32_t LengthRemaining;
	uint8_t * PlayHead;
	bool IsLooping;
	float Volume;
	float Pitch;

	struct Voice * Next;
} Voice;

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
	Voice * GetVoice(const std::string & filename, bool loop, int volume);
private:
	ResourceManager();
	~ResourceManager();
};

#endif // !_RESOURCEMANAGER_H
