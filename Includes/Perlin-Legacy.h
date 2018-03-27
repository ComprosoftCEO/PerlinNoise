//Older implementation of Perlin Noise
#ifndef LEGACY_PERLIN_HEADER
#define LEGACY_PERLIN_HEADER

#include <stdint.h>

typedef void* pLegacy_Perlin_t;

//Variables for perlin data types
typedef uint16_t pLegacySize;


pLegacy_Perlin_t new_legacy_perlin(pLegacySize dimensions);
pLegacy_Perlin_t new_legacy_perlin_seed(pLegacySize dimensions, uint64_t seed);
void free_legacy_perlin(pLegacy_Perlin_t);

//Expects a list of double, equal to the number of coordinates
double legacy_perlin_noise(pLegacy_Perlin_t p, double* coords);


#endif // LEGACY_PERLIN_HEADER Included
