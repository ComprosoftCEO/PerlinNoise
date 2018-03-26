//Main header for DLL external functions
#ifndef PERLIN_NOISE_API_HEADER
#define PERLIN_NOISE_API_HEADER

#include <Platform.h>
#include <Dll.h>

#include <stdint.h>

DLL_EXPORT int init_2d_perlin();
DLL_EXPORT int init_2d_perlin_seed(uint64_t seed);

DLL_EXPORT void release_2d_perlin();
DLL_EXPORT double get_2d_perlin(double x, double y);

#endif // PERLIN_NOISE_API_HEADER Included
