#include "YoManager.h"
#include "YoAudio.h"

YoManager* inst = nullptr;

bool YOA_Init(void)
{
	inst = YoManager::GetInstance();

	return (inst != nullptr); 
}

void YOA_Quit(bool quitSDL)
{ 
	if (inst == nullptr)
		return;

	YoManager::Release(quitSDL);
}

uint16_t YOA_PlayWavFile(const char * filename, bool loop, float volume, float pitch)
{
	if (inst == nullptr)
		return 0u;

	return YoManager::GetInstance()->PlayWavFile(std::string(filename), loop, volume, pitch);
}

int YOA_StopVoice(uint16_t id)
{
	if (inst == nullptr)
		return false;

	return YoManager::GetInstance()->StopVoice(id) ? 0 : 1;
}

void YOA_Pause(void)
{
	if (inst == nullptr)
		return;

	YoManager::GetInstance()->Pause(true);
}

void YOA_Resume(void)
{
	YoManager::GetInstance()->Pause(false);
}
