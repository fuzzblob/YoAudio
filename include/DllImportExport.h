#ifndef _DLL_IMPORT_EXPORT_H
#define _DLL_IMPORT_EXPORT_H

#if defined( BUILD_DLL )
#define YOA_API __declspec(dllexport)
#else
#define YOA_API __declspec(dllimport)
#endif

#endif // !_DLL_IMPORT_EXPORT_H