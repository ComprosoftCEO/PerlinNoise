#include <Rand64.h>


//Define the private object structure
typedef struct {
	uint64_t arr[R64_STATE];		// Stores the internal values
	uint64_t bak_arr[R64_STATE];	// Initial state (for quick reset)
	uint64_t seed;					// Current seed (for quick reset)
	uint64_t index;					// Current location inside the state
} Rand64_Obj, *pRand64_Obj;

static pRand64_Obj internal_rand = NULL;



pRand64 New_Rand64() {
	pRand64 rand = malloc(sizeof(Rand64_Obj));
	Rand64_RandomSeed(rand);

	return (pRand64) rand;
}



pRand64 New_Rand64_Seed(uint64_t seed) {
	pRand64 rand = malloc(sizeof(Rand64_Obj));
	Rand64_Reseed(rand,seed);

	return (pRand64) rand;
}


void Free_Rand64(pRand64 rand) {
    if (rand) {free(rand);}
}


void Rand64_Reseed(pRand64 rand, uint64_t seed) {
    if (!rand) {return;}

	pRand64_Obj rand64 = (pRand64_Obj) rand;
	rand64->seed = seed;

	//Use Linear congruential generator to fill array
	int i;
	for (i = 0; i < R64_STATE; ++i) {
		rand64->arr[i] = seed;
		rand64->bak_arr[i] = seed;
		seed = (seed*R64_MULTIPLER + R64_INCREMENT);

        //Switch bits
        seed = (seed << 32) | (seed >> 32);
	}
	rand64->index = 0;
}


//Pick a random seed
#include <time.h>
void Rand64_RandomSeed(pRand64 rand) {

	//Used to pick random numbers
	static pRand64 rand_seeder = NULL;
	if (!rand_seeder) {rand_seeder = New_Rand64_Seed(time(NULL));}

	Rand64_Reseed(rand,Rand64_Next(rand_seeder));
}



uint64_t Rand64_GetSeed(pRand64 rand) {
    if (!rand) {return -1;}
	return ((pRand64_Obj) rand)->seed;
}


void Rand64_Reset(pRand64 rand) {
	pRand64_Obj rand64 = (pRand64_Obj) rand;
    if (!rand) {return;}

	//Copy from the backup array for quick reset
	int i;
	for (i = 0; i < R64_STATE; ++i) {
		rand64->arr[i] = rand64->bak_arr[i];
	}

	rand64->index = 0;
}




uint64_t Rand64_Next(pRand64 rand) {
	pRand64_Obj r64 = (pRand64_Obj) rand;
    if (!rand) {
        if (!internal_rand) {internal_rand = New_Rand64();}
        r64 = internal_rand;
    }

	uint64_t index = r64->index=(r64->index+1) & R64_MASK;
	r64->arr[index] += (r64->arr[(index + 5) & R64_MASK] + r64->arr[(index + 17) & R64_MASK]);
	return r64->arr[index];
}
