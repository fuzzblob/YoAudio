#ifndef _GUI_H
#define _GUI_H

#include <memory>
#include <SDL.h>
#include "Graphics.h"
#include <imgui.h>

class Gui
{
private:
	const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	std::unique_ptr<Graphics> mGraphics;
	SDL_Window* mWindow;
public:
	Gui() noexcept
	{
		mGraphics = std::make_unique<Graphics>();
		mWindow = mGraphics->GetWindow();
	};
	~Gui()
	{
		mGraphics = nullptr;
		mWindow = nullptr;
	};

	void DemoFrame();
	bool ProcessEvent(SDL_Event* event)
	{
		return mGraphics->ProcessEvent(event);
	};
	void StartFrame() { mGraphics->StartFrame(); };
	void EndFrame() { mGraphics->EndFrame(); };
};

#endif // !_GUI_H
