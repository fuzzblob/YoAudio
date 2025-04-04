#pragma once

#include "EngineConfig.h"

// This is a blob for determining the platform
//#define YOA_SDL			0
#define YOA_WINDOWS		1
#define YOA_MAC			2
#define YOA_UNIX		3

#if defined( _WIN32 )
#define YOA_PLATFORM YOA_WINDOWS
// TODO(maris): implement XAudio2
#define YOA_RENDERER YOA_SDL
#elif defined( __APPLE__ )
#define YOA_PLATFORM YOA_MAC
// TODO(maris): implement CoreAudio
#define YOA_RENDERER YOA_SDL
#elif defined( __linux__ ) // linux
// unix OSes should use SDL - https://www.libsdl.org/
#define YOA_PLATFORM YOA_UNIX
#define YOA_RENDERER YOA_SDL
#elif defined( __unix__ ) // all unices not caught above
// unix OSes should use SDL - https://www.libsdl.org/
#define YOA_PLATFORM YOA_UNIX
#define YOA_RENDERER YOA_SDL
#elif defined(_POSIX_VERSION) // POSIX
// unix OSes should use SDL - https://www.libsdl.org/
#define YOA_PLATFORM YOA_UNIX
#define YOA_RENDERER YOA_SDL
#else
#error Platform detection error!
#endif

// switching audio rendering backends
#if YOA_RENDERER == YOA_SDL
#ifdef SDL2_FOUND
#include <SDL.h>
#else
#undef YOA_RENDERER
#error SDL rendering set but SDL not found!
#endif
#else
#error audio backend for this platform currently not implemented
#endif
