#pragma once

#include "YoaConfig.h"

// This is a blob for switching audio rendering backends
#define YOA_SDL			0
#define YOA_WINDOWS		1
#define YOA_MAC			2
#define YOA_UNIX		3

#if defined( _WIN32 )
#define YOA_PLATFORM YOA_WINDOWS
// TODO: implement XAudio2
#elif defined( __APPLE__ )
#define YOA_PLATFORM YOA_MAC
// TODO: implement CoreAudio
#else
// unix OSes should use SDL - https://www.libsdl.org/
#define YOA_PLATFORM YOA_SDL
#endif

// define this to force SDL backend
#ifdef FORCE_SDL
#undef YOA_PLATFORM
#define YOA_PLATFORM YOA_SDL
#endif

// TODO: handle other platform specific backends
#if YOA_PLATFORM == YOA_SDL
#ifdef SDL2_FOUND
#include <SDL.h>
#else
#undef YOA_PLATFORM
#endif
#else
#error audio backend for this platform currently not implemented
#endif
