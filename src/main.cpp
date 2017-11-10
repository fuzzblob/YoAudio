#include <stdio.h>

#include <SDL.h>

#include "YOA/YOAudio.h"

// have to override SDL defined main for some reson...
#undef main
int main(void)
{
	// initialize YO audio
	if (YOA_Init() != 0)
	{
		fprintf(stderr, "[%s:\t%d]Warning: failed to initilize YoAudio!\n\n", __FILE__, __LINE__);
		return 1;
	}

	// Small demo showing:
	// oneshots, loops, stopping, 
	// pausing & resuming the audio engine

	PlaySound("door_open_01.wav", false, 64);
	SDL_Delay(250);
	uint16_t ambLoop = PlaySound("ambience.wav", true, 128);

	// don't use delays in actual code like this
	SDL_Delay(3500);

	// testing pause functionality
	YOA_Pause();
	SDL_Delay(500);
	YOA_Resume();

	SDL_Delay(500);
	PlaySound("door_close_01.wav", false, 64);
	SDL_Delay(200);
	StopVoice(ambLoop);
	SDL_Delay(750);

	// quit YO audio
	YOA_Quit();

	// TODO: move this to YoAudio (as commented in YOA_Init())
	// quit SDL
	SDL_Quit();

    return 0;
}
