#include "AssetManager.h"

AssetManager* AssetManager::sInstance = nullptr;

AssetManager * AssetManager::GetInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new AssetManager();
	}

	return sInstance;
}

void AssetManager::Release()
{
	if (sInstance == nullptr)
		return;

	delete sInstance;
	sInstance = nullptr;
}

std::string AssetManager::GetPath(std::string sFilename)
{
	std::string fullPath = std::string(SDL_GetBasePath());
	fullPath.append("assets\/" + sFilename);
	return fullPath;
}

Sound * AssetManager::GetSound(const char * cFilename)
{
	std::string fullPath = GetPath(cFilename);

	if (mSounds[fullPath] == nullptr)
	{
		mSounds[fullPath] = LoadSound(fullPath.c_str());
	}

	return mSounds[fullPath];
}

Sound * AssetManager::LoadSound(const char * cFilename)
{
	// TODO: make a map and re-use same sample buffer if possible
	Sound * newSound = (Sound*)malloc(sizeof(Sound));

	if (newSound == nullptr)
	{
		fprintf(stderr, "[%s:\t%d]\nError: Memory allocation error\n\n", __FILE__, __LINE__);
		return nullptr;
	}

	if (SDL_LoadWAV(cFilename, &(newSound->Spec), &(newSound->Buffer), &(newSound->SampleLength)) == nullptr)
	{
		fprintf(stderr, "[%s:\t%d]\nWarning: failed to open wave file: %s error: %s\n\n", __FILE__, __LINE__, cFilename, SDL_GetError());
		free(newSound);
		return nullptr;
	}

	newSound->Buffer = newSound->Buffer;
	newSound->Length = newSound->SampleLength;
	(newSound->Spec).callback = nullptr;
	(newSound->Spec).userdata = nullptr;

	return newSound;
}

bool AssetManager::FreeSound(Sound* sound)
{
	SDL_FreeWAV(sound->Buffer);

	return false;
}

AssetManager::AssetManager()
{
	//
}

AssetManager::~AssetManager()
{
	for (auto snd : mSounds)
	{
		if (snd.second != nullptr)
		{
			SDL_FreeWAV(snd.second->Buffer);
		}
	}
	mSounds.clear();
}
