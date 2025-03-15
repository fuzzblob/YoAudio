#ifndef _SDL_H
#define _SDL_H

// Dear ImGui: standalone example application for SDL2 + OpenGL
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>
#include <SDL.h>
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

namespace YoaEditor
{
	class SDLopenGL
	{
	private:
		SDL_Window* mWindow = nullptr;
		SDL_GLContext mGlContext = nullptr;
	public:
		SDLopenGL() noexcept;
		~SDLopenGL();
		SDL_Window* GetWindow() noexcept {
			return mWindow;
		};

		bool ProcessEvent(const SDL_Event* event)
		{
			// Forward your event to backend
			return ImGui_ImplSDL2_ProcessEvent(event);
		}

		void StartFrame() {
			// (After event loop)
			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();
		};
		void EndFrame()
		{
			// Rendering
			glViewport
			(
				0,
				0,
				int(ImGui::GetIO().DisplaySize.x),
				int(ImGui::GetIO().DisplaySize.y)
			);
			static const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
			glClearColor
			(
				clear_color.x,
				clear_color.y,
				clear_color.z,
				clear_color.w
			);
			glClearColor
			(
				clear_color.x * clear_color.w,
				clear_color.y * clear_color.w,
				clear_color.z * clear_color.w,
				clear_color.w
			);
			glClear(GL_COLOR_BUFFER_BIT);
			// Rendering
			// (Your code clears your framebuffer, renders your other stuff etc.)
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			// (Your code calls SDL_GL_SwapWindow() etc.)
			SDL_GL_SwapWindow(mWindow);
		};

		bool ProcessEvent(SDL_Event* event) { return ImGui_ImplSDL2_ProcessEvent(event); };
	};
}

#endif // !_OPENGL_H
