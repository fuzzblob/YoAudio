#ifndef _YOA_H
#define _YOA_H

#include "YoManager.h"

#ifdef __cplusplus
extern "C"
{
#endif
	int YOA_Init(void) 
	{
		return (YoManager::GetInstance() != nullptr); 
	}

	void YOA_Quit(void) 
	{ 
		YoManager::Release(); 
	}

	uint16_t PlayWavFile(const char * filename, bool loop, float volume, float pitch)
	{
		return YoManager::GetInstance()->PlayWavFile(std::string(filename), loop, volume, pitch);
	}

	int StopVoice(uint16_t id)
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
#ifdef __cplusplus
}
#endif

#endif // _YOA_H
