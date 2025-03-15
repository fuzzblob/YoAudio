#ifndef _EDITOR_H
#define _EDITOR_H

#include "Graphics.h"
#include "EditorConfig.h"

class Editor
{
private:
	// Application
	bool mQuit = false;
	// GUI
	static constexpr uint64_t mTargetFrameLength = 1000u / EDITOR_FRAME_RATE;
	const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	std::unique_ptr<Graphics> mGraphics;
	SDL_Window* mWindow = nullptr;
	// Cleanup function
	void Shutdown();
	// ImGui layouts
	void App();
	void Menu();
public:
	int Run();
};

#endif // !_EDITOR_H
