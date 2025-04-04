#pragma once

#include "DllImportExport.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

bool YOA_API YOA_Init(void);

void YOA_API YOA_Quit(const bool quitSDL);

uint32_t YOA_API YOA_PlayWavFile(const char * filename, const bool loop, const float volume, const float pitch, const float fadeIn, const float pan);

bool YOA_API YOA_StopVoice(const uint32_t id, const float fadeOut);

void YOA_API YOA_SetVoiceVolume(const uint32_t id, const float newVolume);

void YOA_API YOA_SetVoicePan(const uint32_t id, const float newPan);

void YOA_API YOA_Pause(void);

void YOA_API YOA_Resume(void);

void YOA_API YOA_Stop(const float fadeOut);

#ifdef __cplusplus
}
#endif
