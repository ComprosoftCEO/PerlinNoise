#ifndef RAND64_HEADER
#define RAND64_HEADER

#include <stdlib.h>
#include <stdint.h>


#define R64_MULTIPLER		6364136223846793005
#define R64_INCREMENT		1442695040888963407

#define R64_STATE 32
#define R64_MASK  31

typedef void *pRand64;


pRand64 New_Rand64();
pRand64 New_Rand64_Seed(uint64_t seed);
void Free_Rand64(pRand64 rand);

void Rand64_Reseed(pRand64 rand, uint64_t seed);
void Rand64_RandomSeed(pRand64 rand);

uint64_t Rand64_GetSeed(pRand64 rand);
void Rand64_Reset(pRand64 rand);

uint64_t Rand64_Next(pRand64 rand);

#endif // RAND64_HEADER Included
