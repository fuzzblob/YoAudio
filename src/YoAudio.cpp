#include "YoAudio.h"

#include "AudioThread.h"
#include <memory>

bool YOA_Init(void)
{
	return (AudioThread::GetInstance(true) != nullptr);
}

void YOA_Quit(const bool quitSDL)
{ 
	AudioThread::Release(quitSDL);
}

int YOA_PlayWavFile(const char * filename, const bool loop, const float volume, const float pitch, const float fadeIn, const float pan)
{
	AudioThread* inst = AudioThread::GetInstance();
	if (inst != nullptr) {
		return inst->mMixer->PlayWavFile(std::string(filename), loop, volume, pitch, fadeIn, pan);
	}
	return 0u;
}

bool YOA_StopVoice(const int id, const float fadeOut)
{
	AudioThread* inst = AudioThread::GetInstance();
	if (inst != nullptr) {
		return inst->mMixer->StopVoice(uint16_t(id), fadeOut);
	}
	return false;
}

void YOA_API YOA_SetVoiceVolume(const int id, const float newVolume)
{
	AudioThread* inst = AudioThread::GetInstance();
	if (inst != nullptr) {
		inst->mMixer->SetVoiceVolume(id, newVolume);
	}
}

void YOA_API YOA_SetVoicePan(const int id, const float newPan)
{
	AudioThread* inst = AudioThread::GetInstance();
	if (inst != nullptr) {
		inst->mMixer->SetVoicePan(id, newPan);
	}
}

void YOA_Pause(void)
{
	AudioThread* inst = AudioThread::GetInstance();
	if (inst != nullptr) {
		inst->mMixer->Pause(true);
	}
}

void YOA_Resume(void)
{
	AudioThread* inst = AudioThread::GetInstance();
	if (inst != nullptr) {
		inst->mMixer->Pause(false);
	}
}
