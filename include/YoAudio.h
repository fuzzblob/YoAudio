#ifndef _YOA_H
#define _YOA_H

#include "DllImportExport.h"

#ifdef __cplusplus
extern "C"
{
#endif

bool YOA_API YOA_Init(void);

void YOA_API YOA_Quit(bool quitSDL);

uint16_t YOA_API YOA_PlayWavFile(const char * filename, bool loop, float volume, float pitch);

bool YOA_API YOA_StopVoice(uint16_t id);

void YOA_API YOA_Pause(void);

void YOA_API YOA_Resume(void);

#ifdef __cplusplus
}
#endif

#endif // _YOA_H
