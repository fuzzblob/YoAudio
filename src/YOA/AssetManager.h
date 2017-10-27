#ifndef _ASSETMANAGER_H
#define _ASSETMANAGER_H

#include <map>

#include "Defs.h"

#pragma region STRUCTS
typedef struct Sound
{
	uint32_t Length;
	uint32_t SampleLength;
	uint8_t * Buffer;

	SDL_AudioSpec Spec;
} Sound;

typedef struct Voice
{
	uint16_t ID;
	Sound * Sound;
	uint32_t LengthRemaining;
	uint8_t * PlayHead;
	bool IsLooping;
	bool IsStopping;
	uint8_t Volume;

	struct Voice * Next;
} Voice;

typedef struct AudioDevice
{
	SDL_AudioDeviceID DeviceID;
	SDL_AudioSpec SpecWanted;
	SDL_AudioSpec SpecObtained;
	bool IsEnabled;
} AudioDevice;
#pragma endregion SOUND | VOICE | DEVICE

class AssetManager
{
private:
	static AssetManager* sInstance;

	std::map<std::string, Sound*> mSounds;

public:
	static AssetManager* GetInstance();
	static void Release();
	static std::string GetPath(std::string sFilename);

	Sound* GetSound(const char * cFilename);
	Sound * LoadSound(const char * cFilename);
	bool FreeSound(Sound* sound);
private:
	AssetManager();
	~AssetManager();
};

#endif // !_ASSETMANAGER_H
