#ifndef _YOA_H
#define _YOA_H

#include "DllImportExport.h"

#ifdef __cplusplus
extern "C"
{
#endif

bool IMPORT_EXPORT YOA_Init(void);
void IMPORT_EXPORT YOA_Quit(void);
uint16_t IMPORT_EXPORT YOA_PlayWavFile(const char * filename, bool loop, float volume, float pitch);
int IMPORT_EXPORT YOA_StopVoice(uint16_t id);
void IMPORT_EXPORT YOA_Pause(void);
void IMPORT_EXPORT YOA_Resume(void);

#ifdef __cplusplus
}
#endif

#endif // _YOA_H
