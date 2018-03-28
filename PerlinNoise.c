//This is the file that contains the public .dll functions
#include <PerlinNoise.h>
#include <Perlin.h>
#include <stdio.h>

static __thread pPerlin_t perlin = NULL;

DLL_EXPORT int init_2d_perlin() {

    if (perlin) {
        free(perlin);
        perlin = NULL;
    }

    perlin = new_perlin(2);
    return (perlin ? 1 : 0);
}

DLL_EXPORT int init_2d_perlin_seed(uint64_t seed) {
    if (perlin) {
        free_perlin(perlin);
        perlin = NULL;
    }

    perlin = new_perlin_seed(2,seed);
    return (perlin ? 1 : 0);
}

DLL_EXPORT void release_2d_perlin() {
    if (perlin) {
        free_perlin(perlin);
        perlin = NULL;
    }
}


DLL_EXPORT double get_2d_perlin(double x, double y) {
    double arr[2] = {x,y};
    return perlin_noise(perlin,arr);
}
