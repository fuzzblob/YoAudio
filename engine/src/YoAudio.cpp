#include "YoAudio.h"

#include "AudioThread.h"
#include <memory>

bool YOA_Init(void)
{
	return bool(AudioThread::GetInstance(true));
}

void YOA_Quit(const bool quitSDL)
{ 
	AudioThread::Release(quitSDL);
}

uint32_t YOA_PlayWavFile(const char * filename, const bool loop, const float volume, const float pitch, const float fadeIn, const float pan)
{
	const AudioThread* inst = AudioThread::GetInstance();
	if (inst) {
		return inst->mMixer->PlayWavFile(std::string(filename), loop, volume, pitch, fadeIn, pan);
	}
	return 0u;
}

bool YOA_StopVoice(const uint32_t id, const float fadeOut)
{
	const AudioThread* inst = AudioThread::GetInstance();
	if (inst) {
		return inst->mMixer->StopVoice(id, fadeOut);
	}
	return false;
}

void YOA_API YOA_SetVoiceVolume(const uint32_t id, const float newVolume)
{
	const AudioThread* inst = AudioThread::GetInstance();
	if (inst) {
		inst->mMixer->SetVoiceVolume(id, newVolume);
	}
}

void YOA_API YOA_SetVoicePan(const uint32_t id, const float newPan)
{
	const AudioThread* inst = AudioThread::GetInstance();
	if (inst) {
		inst->mMixer->SetVoicePan(id, newPan);
	}
}

void YOA_Pause(void)
{
	const AudioThread* inst = AudioThread::GetInstance();
	if (inst) {
		inst->mMixer->Pause(true);
	}
}

void YOA_Resume(void)
{
	const AudioThread* inst = AudioThread::GetInstance();
	if (inst) {
		inst->mMixer->Pause(false);
	}
}
