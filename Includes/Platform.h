//Used to figure out what platform that I am running on???
//
//  Defines one or more of the following symbols:
//      PLAT_WINDOWS - Microsoft windows
//      PLAT_UNIX    - Unix-Based Platform (may also define LINUX)
//      PLAT_LINUX   - Linux operating system
//      PLAT_MACOSX  - Apple Operating system

#ifndef PLATFORM_HEADER
#define PLATFORM_HEADER

#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64) || defined(__MS_DOS__)
    #define PLAT_WINDOWS
#endif


#if defined(__unix__) || defined(unix) || defined(__unix)
    #define PLAT_UNIX
#endif

#if defined(__linux__) || defined(linux) || defined(__linux)
    #define PLAT_LINUX
#endif


#if defined(__APPLE__) || defined(__MACH__)
    #define PLAT_MACOSX
#endif


//Test for supported platforms

#if (!defined(PLAT_LINUX)) && (!defined(PLAT_WINDOWS))
    #error "This platform is not supported, yet"
#endif


#endif // PLATFORM_HEADER Included
