#include "Graphics.h"

#include "imgui\imgui.h"
#include "imgui\GL\imgui_impl_sdl_gl3.h"
#include <stdio.h>
#include "imgui\GL\gl3w.h"

Graphics* Graphics::sInstance = nullptr;
bool Graphics::sInitialized = false;

Graphics* Graphics::GetInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new Graphics();
	}

	return sInstance;
}

bool Graphics::IsInitialized()
{
	return (sInstance != nullptr);
}

void Graphics::Release()
{
	delete sInstance;
	sInstance = nullptr;
}

Graphics::Graphics()
{
	//mBackBuffer = nullptr;
	sInitialized = Init();
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

bool Graphics::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL Video Initialization Error: %s\n", SDL_GetError());
		return false;
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
	mWindow = SDL_CreateWindow("Yo Audio Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	mGlContext = SDL_GL_CreateContext(mWindow);
	gl3wInit();

	// Setup ImGui binding
	ImGui_ImplSdlGL3_Init(mWindow);

	// Setup style
	ImGui::StyleColorsClassic();
	//ImGui::StyleColorsDark();

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them. 
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple. 
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'extra_fonts/README.txt' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//ImGuiIO& io = ImGui::GetIO();
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	return true;
}

SDL_Window * Graphics::GetWindow()
{
	return mWindow;
}
