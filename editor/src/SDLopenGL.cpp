#include "SDLopenGL.h"

#include <SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

namespace YoaEditor
{
    SDLopenGL::SDLopenGL() noexcept
    {
        // Setup SDL
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
        {
            printf("Error: %s\n", SDL_GetError());
            return;
        }
        // Create window with graphics context
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_WindowFlags window_flags = static_cast<SDL_WindowFlags>(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
        mWindow = SDL_CreateWindow("YoAudio Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
        if (mWindow == nullptr)
        {
            printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
            return;
        }

        mGlContext = SDL_GL_CreateContext(mWindow);
        if (mGlContext == nullptr)
        {
            printf("Error: SDL_GL_CreateContext(): %s\n", SDL_GetError());
            return;
        }

        SDL_GL_MakeCurrent(mWindow, mGlContext);
        SDL_GL_SetSwapInterval(1); // Enable vsync

        // Setup Platform/Renderer backends
        ImGui_ImplSDL2_InitForOpenGL(mWindow, mGlContext);
        ImGui_ImplOpenGL3_Init();
    }

    SDLopenGL::~SDLopenGL()
    {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        SDL_GL_DeleteContext(mGlContext);
        SDL_DestroyWindow(mWindow);
        SDL_Quit();
    }

    SDL_Window* SDLopenGL::GetWindow() noexcept
    {
        return mWindow;
    };
    bool ProcessEvent(const SDL_Event* event)
    {
        // Forward your event to backend
        return ImGui_ImplSDL2_ProcessEvent(event);
    }
    void SDLopenGL::StartFrame() {
        // (After event loop)
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    };

    void SDLopenGL::EndFrame()
    {
        // setup
        glViewport(
            0, 0,
            static_cast<int>(ImGui::GetIO().DisplaySize.x),
            static_cast<int>(ImGui::GetIO().DisplaySize.y)
            );
        static const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        // TODO(maris): this probably doesnt need to happen twice in a row
        glClearColor(
            clear_color.x,
            clear_color.y,
            clear_color.z,
            clear_color.w
            );
        // clear the frame buffer
        glClearColor(
            clear_color.x * clear_color.w,
            clear_color.y * clear_color.w,
            clear_color.z * clear_color.w,
            clear_color.w
            );
        glClear(GL_COLOR_BUFFER_BIT);
        // Rendering
        ImGui::Render();
        // pass ImGui frame to OpenGL to draw
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // call SDL_GL_SwapWindow()
        SDL_GL_SwapWindow(mWindow);
    };
}  // namespace YoaEditor
