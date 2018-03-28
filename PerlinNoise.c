//This is the file that contains the public .dll functions
#include <PerlinNoise.h>
#include <Perlin.h>
#include <stdio.h>


static __thread pPerlin_t perlin_1d = NULL;
static __thread pPerlin_t perlin_2d = NULL;
static __thread pPerlin_t perlin_3d = NULL;

DLL_EXPORT int init_perlin() {
    if (!perlin_1d) {if (!(perlin_1d = new_perlin(1))) {return -1;}}
    if (!perlin_2d) {if (!(perlin_2d = new_perlin(2))) {return -2;}}
    if (!perlin_3d) {if (!(perlin_3d = new_perlin(3))) {return -3;}}
    return 0;
}

DLL_EXPORT void release_perlin() {
    if (perlin_1d) {free_perlin(perlin_1d); perlin_1d = NULL;}
    if (perlin_2d) {free_perlin(perlin_2d); perlin_2d = NULL;}
    if (perlin_3d) {free_perlin(perlin_3d); perlin_3d = NULL;}
}

//Check if Perlin Noise is properly initialized
DLL_EXPORT int perlin_is_init() {
    return (perlin_1d && perlin_2d && perlin_3d);
}

DLL_EXPORT void seed_perlin_1d(uint64_t seed) {
    if (perlin_1d) {reseed_perlin(perlin_1d,seed);}
}
DLL_EXPORT void seed_perlin_2d(uint64_t seed) {
    if (perlin_2d) {reseed_perlin(perlin_2d,seed);}
}
DLL_EXPORT void seed_perlin_3d(uint64_t seed) {
    if (perlin_3d) {reseed_perlin(perlin_3d,seed);}
}


DLL_EXPORT void random_perlin_1d() {
    if (perlin_1d) {random_seed_perlin(perlin_1d);}
}
DLL_EXPORT void random_perlin_2d() {
    if (perlin_2d) {random_seed_perlin(perlin_2d);}
}
DLL_EXPORT void random_perlin_3d() {
    if (perlin_3d) {random_seed_perlin(perlin_3d);}
}



DLL_EXPORT double get_1d_perlin(double x) {
    return perlin_noise(perlin_1d,&x);
}

DLL_EXPORT double get_2d_perlin(double x, double y) {
    double arr[2] = {x,y};
    return perlin_noise(perlin_2d,arr);
}
DLL_EXPORT double get_3d_perlin(double x, double y, double z) {
    double arr[3] = {x,y,z};
    return perlin_noise(perlin_3d,arr);
}



DLL_EXPORT double get_1d_perlin_octave(double x, uint32_t octaves, double persistance) {
    return perlin_noise_octave(perlin_1d,&x,octaves,persistance);
}
DLL_EXPORT double get_2d_perlin_octave(double x, double y, uint32_t octaves, double persistance) {
    double arr[2] = {x,y};
    return perlin_noise_octave(perlin_2d,arr,octaves,persistance);
}
DLL_EXPORT double get_3d_perlin_octave(double x, double y, double z, uint32_t octaves, double persistance) {
    double arr[3] = {x,y,z};
    return perlin_noise_octave(perlin_3d,arr,octaves,persistance);
}
