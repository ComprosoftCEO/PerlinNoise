//Main header for DLL external functions
#ifndef PERLIN_NOISE_API_HEADER
#define PERLIN_NOISE_API_HEADER

#include <Platform.h>
#include <Dll.h>

#include <stdint.h>

DLL_EXPORT int init_perlin();
DLL_EXPORT void release_perlin();
DLL_EXPORT int perlin_is_init();

DLL_EXPORT void seed_perlin_1d(uint64_t seed);
DLL_EXPORT void seed_perlin_2d(uint64_t seed);
DLL_EXPORT void seed_perlin_3d(uint64_t seed);

DLL_EXPORT void random_perlin_1d();
DLL_EXPORT void random_perlin_2d();
DLL_EXPORT void random_perlin_3d();

DLL_EXPORT double get_1d_perlin(double x);
DLL_EXPORT double get_2d_perlin(double x, double y);
DLL_EXPORT double get_3d_perlin(double x, double y, double z);

DLL_EXPORT double get_1d_perlin_octave(double x, uint32_t octaves, double persistance);
DLL_EXPORT double get_2d_perlin_octave(double x, double y, uint32_t octaves, double persistance);
DLL_EXPORT double get_3d_perlin_octave(double x, double y, double z, uint32_t octaves, double persistance);

#endif // PERLIN_NOISE_API_HEADER Included
