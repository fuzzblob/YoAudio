#ifndef _EDITOR_H
#define _EDITOR_H

#include "Graphics.h"

class Editor
{
private:
	// Application
	bool mQuit = false;
	// GUI
	const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	std::unique_ptr<Graphics> mGraphics;
	SDL_Window* mWindow = nullptr;
	// ImGui layouts
	void App();
	void Menu();
public:
	void Run();
};

#endif // !_EDITOR_H
