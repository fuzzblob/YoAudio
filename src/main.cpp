#include <stdio.h>

#include <SDL.h>

#include "YOA\YOAudio.h"

// have to override SDL defined main for some reson...
#undef main
int main(void)
{
	// initialize SDL audio
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		return 1;
	}

	// initialize YO audio
	YOA_Init();

	// Small demo showing:
	// oneshots, loops, stopping, 
	// pausing & resuming the audio engine

	PlaySound("door_open_01.wav", false, SDL_MIX_MAXVOLUME / 2);
	SDL_Delay(250);
	uint16_t ambLoop = PlaySound("ambience.wav", true, SDL_MIX_MAXVOLUME);

	// don't use delays in actual code like this
	SDL_Delay(3500);

	// testing pause functionality
	YOA_Pause();
	SDL_Delay(500);
	YOA_Resume();

	SDL_Delay(500);
	PlaySound("door_close_01.wav", false, SDL_MIX_MAXVOLUME / 2);
	SDL_Delay(200);
	StopVoice(ambLoop);
	SDL_Delay(750);

	// quit YO audio
	YOA_Quit();

	// quit SDL
	SDL_Quit();

    return 0;
}
