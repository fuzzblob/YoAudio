#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <SDL.h>
#include <stdio.h>
#include <string>

class Graphics
{
private:
	SDL_Window* mWindow;
	SDL_GLContext mGlContext;
public:
	const int ScreenWidth = 1280;
	const int ScreenHeight = 720;

	SDL_Window* GetWindow();
	Graphics();
	~Graphics();
};

#endif // !_GRAPHICS_H