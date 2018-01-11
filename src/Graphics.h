#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <SDL.h>
#include <stdio.h>
#include <string>

class Graphics
{
public:
	static const int ScreenWidth = 1280;
	static const int ScreenHeight = 720;
private:
	static Graphics* sInstance;
	static bool sInitialized;

	SDL_Window* mWindow;
	SDL_GLContext mGlContext;

public:
	static Graphics* GetInstance();
	static bool IsInitialized();
	static void Release();

	SDL_Window* GetWindow();
private:
	Graphics();
	~Graphics();

	bool Init(); 
};

#endif // !_GRAPHICS_H