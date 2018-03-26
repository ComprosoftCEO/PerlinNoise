#ifndef PERLIN_HEADER
#define PERLIN_HEADER

#include <stdint.h>

typedef void* pPerlin_t;

//Variables for perlin data types
typedef double pCoord;
typedef uint16_t pSize;


pPerlin_t new_perlin(pSize dimensions);
pPerlin_t new_perlin_seed(pSize dimensions, uint64_t seed);
void free_perlin(pPerlin_t);

//Expects a list of double, equal to the number of coordinates
double perlin_noise(pPerlin_t p, double* coords);


#endif // PERLIN_HEADER Included
