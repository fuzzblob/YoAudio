#ifndef _SDL_H
#define _SDL_H

// Dear ImGui: standalone example application for SDL2 + OpenGL
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include <SDL.h>

namespace YoaEditor
{
	class SDLopenGL
	{
	private:
		SDL_Window* mWindow = nullptr;
		SDL_GLContext mGlContext = nullptr;
	public:
		SDLopenGL() noexcept;
		SDLopenGL(const SDLopenGL &) = delete;
		SDLopenGL(SDLopenGL &&) = delete;
		SDLopenGL &operator=(const SDLopenGL &) = delete;
		SDLopenGL &operator=(SDLopenGL &&) = delete;
		~SDLopenGL();

		SDL_Window* GetWindow() noexcept;
		bool ProcessEvent(const SDL_Event* event);
		void StartFrame();
		void EndFrame();
	};
}  // namespace YoaEditor

#endif // !_OPENGL_H
