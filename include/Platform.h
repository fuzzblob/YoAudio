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

enum SampleFormat
{
	YOA_Format_Unknown = -1,
	YOA_Format_Float = 0,
	YOA_Format_Sint8 = 8,
	YOA_Format_Sint16 = 16,
	YOA_Format_Sint32 = 32
};

// Here are some builtin types and their value ranges
/*
#define SInt8 signed char;				// '\x80' to '\x7f' (-128 to 127) 
#define SInt16 signed short int;		// -32768 to 32767
#define SInt32 signed int;				// Usually	-2147483648		to 2147483647
#define SInt64 signed long int;			// At least	-2147483648l	to 2147483647L
#define UInt8 unsigned char;			// '\x00' to '\XFF' (0 to 255)
#define UInt16 unsigned short int;		// 0u to 65535U
#define UInt32 unsigned int;			// Usually 0 to		4294967295U
#define UInt64 unsigned long int;		// 0ul to at least	4294967295LU
#define Float32 signed float;			// -1.7e38f to 1.7E38F, 6 significant digits
#define Float64 signed double;			// -1.8e308 to 1.8E308, 14 significant digits
#define Float80 signed long	double;		// At least double (up to 80bit)
*/
