#include "YoManager.h"
#include "YoAudio.h"
#include <memory>

bool YOA_Init(void)
{
	return (YoManager::GetInstance() != nullptr);
}

void YOA_Quit(const bool quitSDL)
{ 
	YoManager::Release(quitSDL);
}

int YOA_PlayWavFile(const char * filename, const bool loop, const float volume, const float pitch, const float fadeIn)
{
	return YoManager::GetInstance()->PlayWavFile(std::string(filename), loop, volume, pitch, fadeIn);
}

bool YOA_StopVoice(const int id, const float fadeOut)
{
	return YoManager::GetInstance()->StopVoice(id, fadeOut);
}

void YOA_Pause(void)
{
	YoManager::GetInstance()->Pause(true);
}

void YOA_Resume(void)
{
	YoManager::GetInstance()->Pause(false);
}
