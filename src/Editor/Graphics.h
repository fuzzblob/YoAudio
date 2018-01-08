#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

class Graphics
{
public:
	static const int ScreenWidth = 800;
	static const int ScreenHeight = 600;
private:
	static Graphics* sInstance;
	static bool sInitialized;

	SDL_Window* mWindow;
	SDL_Surface* mBackBuffer;

	SDL_Renderer* mRenderer;

public:
	static Graphics* GetInstance();
	static bool IsInitialized();
	static void Release();

	SDL_Texture* LoadTexture(std::string path);

	void ClearBackBuffer();
	void DrawTexture(SDL_Texture* tex, SDL_Rect* rend = nullptr);
	void Render();
	static int GuiRun();
private:
	Graphics();
	~Graphics();

	bool Init(); 
};

#endif // !_GRAPHICS_H