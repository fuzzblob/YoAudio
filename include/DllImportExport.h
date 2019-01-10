#pragma once

#if defined(_MSC_VER)
    //  Microsoft 
    #define YOA_EXPORT __declspec(dllexport)
    #define YOA_IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
    //  GCC
    #define YOA_EXPORT __attribute__((visibility("default")))
    #define YOA_IMPORT
#else
    //  do nothing and hope for the best?
    #define YOA_EXPORT
    #define YOA_IMPORT
    #pragma warning Unknown dynamic link import/export semantics.
#endif


#if defined( BUILD_DLL )
#define YOA_API YOA_EXPORT
#else
#define YOA_API YOA_IMPORT
#endif
