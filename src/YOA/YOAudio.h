#ifndef _YOA_H
#define _YOA_H

#ifdef __cplusplus
extern "C"
{
#endif
	int YOA_Init(void);

	void YOA_Quit(void);

	uint16_t PlaySound(const char * filename, bool loop, int volume);

	void StopVoice(uint16_t id);

	void YOA_Pause(void);

	void YOA_Resume(void);
#ifdef __cplusplus
}
#endif

#endif // _YOA_H
