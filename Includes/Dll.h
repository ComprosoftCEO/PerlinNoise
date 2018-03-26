#ifndef DLL_HEADER
#define DLL_HEADER

#include <Platform.h>

//Specify C++ start and end
#ifdef __cplusplus
  #define C_HDR_START extern "C" {
#else
  #define C_HDR_START
#endif

#ifdef __cplusplus
  #define C_HDR_END }
#else
  #define C_HDR_END
#endif


// Used for Windows, when to build a .dll file
//  Does nothing on Linux
#if defined(_WIN32) || defined(_WIN64)

    #include <windows.h>

    #ifdef TESTAPP
        //Debug should not mangle the file
        #define DLL_EXPORT
    #else
        //Use the same header for the dll and the application
        #ifdef BUILD_DLL
            #define DLL_EXPORT __declspec(dllexport)
        #else
            #define DLL_EXPORT __declspec(dllimport)
        #endif
    #endif

#else
    //This is a Linux OS, so do nothing
    #define DLL_EXPORT
#endif

#endif // DLL_HEADER Included
