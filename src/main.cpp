#include <stdio.h>

#include "YOA\YoManager.h"

// have to override SDL defined main for some reson...
#undef main
int main(void)
{
	YoManager* mAudio = YoManager::GetInstance();

	// play one-shot sound effect
	mAudio->PlayWavFile("door_open_01.wav", false, 0.5f, 1.0f);
	SDL_Delay(250);

	// play looping sound, store voice ID to stop it at a later point
	uint16_t ambLoop = mAudio->PlayWavFile("ambience.wav", true, 1.0f, 1.0f);

	SDL_Delay(3500);

	// pause & unpause the audio engine
	mAudio->Pause(true);
	SDL_Delay(500);
	mAudio->Pause(false);

	SDL_Delay(1200);
	mAudio->PlayWavFile("door_close_01.wav", false, 0.5f, 1.0f);
	SDL_Delay(400);

	// stop the looping sound
	if (mAudio->StopVoice(ambLoop) == false)
	{
		fprintf(stderr, "[%s:\t%d]\nError: could not stop amb-loop!\n\n", __FILE__, __LINE__);
	}
	
	SDL_Delay(1500);

	// quite Yo audio system
	YoManager::Release(true); // also quit SDL
	mAudio = nullptr;

    return 0;
}
