#include "YoManager.h"
#include "YoAudio.h"

bool YOA_Init(void)
{
	return (YoManager::GetInstance() != nullptr); 
}

void YOA_Quit(void)
{ 
	YoManager::Release(true); 
}

uint16_t YOA_PlayWavFile(const char * filename, bool loop, float volume, float pitch)
{
	return YoManager::GetInstance()->PlayWavFile(std::string(filename), loop, volume, pitch);
}

int YOA_StopVoice(uint16_t id)
{
	bool sucess = YoManager::GetInstance()->StopVoice(id);
	if (sucess) return 0;
	else return 1;
}

void YOA_Pause(void)
{
	YoManager::GetInstance()->Pause(true);
}

void YOA_Resume(void)
{
	YoManager::GetInstance()->Pause(false);
}
