#pragma once

#if defined( BUILD_DLL )
#define YOA_API __declspec(dllexport)
#else
#define YOA_API __declspec(dllimport)
#endif
