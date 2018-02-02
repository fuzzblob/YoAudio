#include "Graphics.h"
#include "imgui\imgui.h"
#include "imgui\GL\imgui_impl_sdl_gl3.h"
#include <stdio.h>
#include "imgui\GL\gl3w.h"

Graphics::Graphics()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) // SDL_WasInit(SDL_INIT_VIDEO)
	{
		printf("SDL Video Initialization Error: %s\n", SDL_GetError());
		return;
	}

	// IMGUI setup

	// Setup window
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);
	mWindow = SDL_CreateWindow("Yo Audio Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 650, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	mGlContext = SDL_GL_CreateContext(mWindow);
	gl3wInit();

	// Setup ImGui binding
	ImGui_ImplSdlGL3_Init(mWindow);

	// Setup style
	//ImGui::StyleColorsClassic();
	ImGui::StyleColorsDark();
}

Graphics::~Graphics()
{
	// Cleanup
	ImGui_ImplSdlGL3_Shutdown();
	SDL_GL_DeleteContext(mGlContext);
	SDL_DestroyWindow(mWindow);

	SDL_DestroyWindow(mWindow);
	mWindow = nullptr;

	SDL_Quit();
}

SDL_Window * Graphics::GetWindow()
{
	return mWindow;
}
