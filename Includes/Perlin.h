#ifndef PERLIN_HEADER
#define PERLIN_HEADER

#include <stdint.h>

typedef void* pPerlin_t;

//Variables for perlin data types
typedef uint16_t pSize;


pPerlin_t new_perlin(pSize dimensions);
pPerlin_t new_perlin_seed(pSize dimensions, uint64_t seed);
void reseed_perlin(pPerlin_t p, uint64_t seed);
void random_seed_perlin(pPerlin_t p);
void free_perlin(pPerlin_t);

//Expects a list of double, equal to the number of coordinates
double perlin_noise(pPerlin_t p, const double* coords);
double perlin_noise_octave(pPerlin_t p, const double* coords, uint32_t octaves, double persistence);

#endif // PERLIN_HEADER Included
