#ifndef _DLL_IMPORT_EXPORT_H
#define _DLL_IMPORT_EXPORT_H

#if defined( BUILD_DLL )
#define IMPORT_EXPORT __declspec(dllexport)
#else
#define IMPORT_EXPORT __declspec(dllimport)
#endif

#endif // !_DLL_IMPORT_EXPORT_H