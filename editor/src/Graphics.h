#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "SDLopenGL.h"

#include <imgui.h>
#include <memory>

namespace YoaEditor
{
	class Graphics
	{
	private:
		std::unique_ptr<SDLopenGL> mImplementation;
	public:
		SDL_Window* GetWindow() noexcept {
			return mImplementation->GetWindow();
		};
		Graphics() noexcept
		{
			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

			mImplementation = std::make_unique<SDLopenGL>();
		};
		Graphics(const Graphics &) = delete;
		Graphics(Graphics &&) = delete;
		Graphics &operator=(const Graphics &) = delete;
		Graphics &operator=(Graphics &&) = delete;
		~Graphics() { mImplementation = nullptr; };

		bool ProcessEvent(const SDL_Event* event)
		{
			return mImplementation->ProcessEvent(event);
		};
		void StartFrame() { mImplementation->StartFrame(); };
		void EndFrame() { mImplementation->EndFrame(); };
	};
}  // namespace YoaEditor
#endif // !_GRAPHICS_H
