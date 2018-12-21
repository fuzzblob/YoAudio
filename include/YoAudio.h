#ifndef _YOA_H
#define _YOA_H

#include "DllImportExport.h"

#ifdef __cplusplus
extern "C"
{
#endif

bool YOA_API YOA_Init(void);

void YOA_API YOA_Quit(const bool quitSDL);

uint16_t YOA_API YOA_PlayWavFile(const char * filename, const bool loop, const float volume, const float pitch, const float fadeIn);

bool YOA_API YOA_StopVoice(const uint16_t id, const float fadeOut);

void YOA_API YOA_Pause(void);

void YOA_API YOA_Resume(void);

#ifdef __cplusplus
}
#endif

#endif // _YOA_H
