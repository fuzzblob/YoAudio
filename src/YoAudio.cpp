#include "YoManager.h"
#include "YoAudio.h"
#include <memory>

bool YOA_Init(void)
{
	return (YoManager::GetInstance() != nullptr);
}

void YOA_Quit(bool quitSDL)
{ 
	YoManager::Release(quitSDL);
}

uint16_t YOA_PlayWavFile(const char * filename, bool loop, float volume, float pitch)
{
	return YoManager::GetInstance()->PlayWavFile(std::string(filename), loop, volume, pitch);
}

int YOA_StopVoice(uint16_t id)
{
	return YoManager::GetInstance()->StopVoice(id) ? 1 : 0;
}

void YOA_Pause(void)
{
	YoManager::GetInstance()->Pause(true);
}

void YOA_Resume(void)
{
	YoManager::GetInstance()->Pause(false);
}
