#ifndef _GUI_H
#define _GUI_H

#include <memory>
#include <SDL.h>
#include "Graphics.h"
#include "imgui\imgui.h"

class Gui
{
private:
	const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	std::unique_ptr<Graphics> mGraphics;
	SDL_Window* mWindow;
public:
	Gui() noexcept;
	~Gui();

	void StartFrame();
	void EndFrame();
	void DemoFrame();

	bool ProcessEvent(SDL_Event* event);
};

#endif // !_GUI_H
