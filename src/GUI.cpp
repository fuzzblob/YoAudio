#include "GUI.h"
#include "imgui\GL\imgui_impl_sdl_gl3.h"
#include "imgui\GL\gl3w.h"

Gui::Gui() noexcept 
{
	mGraphics = std::make_unique<Graphics>();
	mWindow = mGraphics->GetWindow();
}

Gui::~Gui()
{
	mGraphics = nullptr;
	mWindow = nullptr;
}

void Gui::StartFrame()
{
	ImGui_ImplSdlGL3_NewFrame(mWindow);
}

void Gui::EndFrame()
{
	// Rendering
	
	glViewport
	(
		0, 
		0, 
		int(ImGui::GetIO().DisplaySize.x),
		int(ImGui::GetIO().DisplaySize.y)
	);
	
	glClearColor
	(
		clear_color.x, 
		clear_color.y, 
		clear_color.z, 
		clear_color.w
	);
	
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui::Render();
	SDL_GL_SwapWindow(mWindow);
}

void Gui::DemoFrame()
{
	StartFrame();

	// 1. Show a simple window.
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
	{
		static float f = 0.0f;
		ImGui::Text("Hello, world!");                           // Some text (you can use a format string too)
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float as a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats as a color
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	// 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow().
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
		bool show = true;
		ImGui::ShowDemoWindow(&show);
	}

	EndFrame();
}

bool Gui::ProcessEvent(SDL_Event * event)
{
	return ImGui_ImplSdlGL3_ProcessEvent(event);
}
