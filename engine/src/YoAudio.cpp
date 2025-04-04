#include "../include/YoAudio.h"
#include "AudioThread.h"

bool YOA_Init(void)
{
	return static_cast<bool>(YoaEngine::AudioThread::GetInstance(true));
}

void YOA_Quit(const bool quitSDL)
{
	YoaEngine::AudioThread::Release(quitSDL);
}

uint32_t YOA_PlayWavFile(const char * filename, const bool loop, const float volume, const float pitch, const float fadeIn, const float pan)
{
	const YoaEngine::AudioThread* inst = YoaEngine::AudioThread::GetInstance();
	if (inst) {
		return inst->GetMixer()->PlayWavFile(std::string(filename), loop, volume, pitch, fadeIn, pan);
	}
	return 0u;
}

bool YOA_StopVoice(const uint32_t id, const float fadeOut)
{
	const YoaEngine::AudioThread* inst = YoaEngine::AudioThread::GetInstance();
	if (inst) {
		return inst->GetMixer()->StopVoice(id, fadeOut);
	}
	return false;
}

void YOA_SetVoiceVolume(const uint32_t id, const float newVolume)
{
	const YoaEngine::AudioThread* inst = YoaEngine::AudioThread::GetInstance();
	if (inst) {
		inst->GetMixer()->SetVoiceVolume(id, newVolume);
	}
}

void YOA_SetVoicePan(const uint32_t id, const float newPan)
{
	const YoaEngine::AudioThread* inst = YoaEngine::AudioThread::GetInstance();
	if (inst) {
		inst->GetMixer()->SetVoicePan(id, newPan);
	}
}

void YOA_Pause(void)
{
	const YoaEngine::AudioThread* inst = YoaEngine::AudioThread::GetInstance();
	if (inst) {
		inst->GetMixer()->Pause(true);
	}
}

void YOA_Resume(void)
{
	const YoaEngine::AudioThread* inst = YoaEngine::AudioThread::GetInstance();
	if (inst) {
		inst->GetMixer()->Pause(false);
	}
}

void YOA_Stop(const float fadeOut)
{
	const YoaEngine::AudioThread* inst = YoaEngine::AudioThread::GetInstance();
	if (inst) {
		inst->GetMixer()->StopSound(fadeOut);
	}
}
