#ifndef _GRAPHICS_H
#define _GRAPHICS_H

// ImGui - standalone example application for SDL2 + OpenGL
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)
// (GL3W is a helper library to access OpenGL functions since there is no standard header to access modern OpenGL functions easily. Alternatives are GLEW, Glad, etc.)

#include "imgui\imgui.h"
#include "imgui\GL\imgui_impl_sdl_gl3.h"
#include <stdio.h>
#include <SDL.h>

class Graphics
{
public:

private:
	Graphics();
	~Graphics();
};

#endif // !_GRAPHICS_H