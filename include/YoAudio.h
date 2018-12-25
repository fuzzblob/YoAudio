#pragma once

#include "DllImportExport.h"

#ifdef __cplusplus
extern "C"
{
#endif

bool YOA_API YOA_Init(void);

void YOA_API YOA_Quit(const bool quitSDL);

int YOA_API YOA_PlayWavFile(const char * filename, const bool loop, const float volume, const float pitch, const float fadeIn);

bool YOA_API YOA_StopVoice(const int id, const float fadeOut);

void YOA_API YOA_SetVoiceVolume(const int id, const float newVolume);

void YOA_API YOA_Pause(void);

void YOA_API YOA_Resume(void);

#ifdef __cplusplus
}
#endif
