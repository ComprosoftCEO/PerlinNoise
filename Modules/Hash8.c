#include <Hash8.h>
#include <Rand64.h>
#include <string.h>

// 0-255 shuffled in any (random) order suffices
static const uint8_t defaultHashTable[256] = {
       98,  6, 85,150, 36, 23,112,164,135,207,169,  5, 26, 64,165,219, //  1
       61, 20, 68, 89,130, 63, 52,102, 24,229,132,245, 80,216,195,115, //  2
       90,168,156,203,177,120,  2,190,188,  7,100,185,174,243,162, 10, //  3
      237, 18,253,225,  8,208,172,244,255,126,101, 79,145,235,228,121, //  4
      123,251, 67,250,161,  0,107, 97,241,111,181, 82,249, 33, 69, 55, //  5
       59,153, 29,  9,213,167, 84, 93, 30, 46, 94, 75,151,114, 73,222, //  6
      197, 96,210, 45, 16,227,248,202, 51,152,252,125, 81,206,215,186, //  7
       39,158,178,187,131,136,  1, 49, 50, 17,141, 91, 47,129, 60, 99, //  8
      154, 35, 86,171,105, 34, 38,200,147, 58, 77,118,173,246, 76,254, //  9
      133,232,196,144,198,124, 53,  4,108, 74,223,234,134,230,157,139, // 10
      189,205,199,128,176, 19,211,236,127,192,231, 70,233, 88,146, 44, // 11
      183,201, 22, 83, 13,214,116,109,159, 32, 95,226,140,220, 57, 12, // 12
      221, 31,209,182,143, 92,149,184,148, 62,113, 65, 37, 27,106,166, // 13
        3, 14,204, 72, 21, 41, 56, 66, 28,193, 40,217, 25, 54,179,117, // 14
      238, 87,240,155,180,170,242,212,191,163, 78,218,137,194,175,110, // 15
       43,119,224, 71,122,142, 42,160,104, 48,247,103, 15, 11,138,239  // 16
};

typedef struct {
    uint8_t hashTable[256];
} Hash8_Obj, *pHash8_Obj;



//Shuffle an array of bytes of a given length
static void Shuffle_Bytes(pRand64 rand, uint8_t* data, size_t len) {
	char temp;

	size_t i;
	for (i = 0; i < len; ++i) {

		//Do some fancy pointer maths to make this swapping very quick
		uint64_t index = Rand64_Next(rand) % (len - i);

		temp = *data;
		*data = data[index];
		data[index] = temp;

		++data;
	}
}

//Pass a Null rand pointer to initialize with default table
pHash8 New_Hash8(pRand64 rand) {
    pHash8_Obj hash = malloc(sizeof(Hash8_Obj));
    Hash8_Reseed((pHash8) hash,rand);
    return (pHash8) hash;
}

void Hash8_Reseed(pHash8 h, pRand64 rand) {
    if (!h) {return;}

    pHash8_Obj hash = (pHash8_Obj) h;
    int i;

    if (rand) {
        for (i = 0; i < 256; ++i) {
            hash->hashTable[i] = (uint8_t) i;
        }
        Shuffle_Bytes(rand,(uint8_t *) hash->hashTable,256);

    //Just use the default table
    } else {
        for (i = 0; i < 256; ++i) {
            hash->hashTable[i] = defaultHashTable[i];
        }
    }
}



void Free_Hash8(pHash8 hash) {
    if (!hash) {return;}
    free(hash);
}


//Use preprocessor like a template
#define HASH8_LENGTH_TEMPLATE(name,type) \
type Hash8_##name##_Length(pHash8 h, const char* str, size_t len) { \
    \
    pHash8_Obj hash = (pHash8_Obj) h;\
	type retVal = 0; \
	uint8_t hashChar; \
	const uint8_t *hashTable = defaultHashTable; \
	size_t i, j; \
    \
    if (hash) {hashTable = hash->hashTable;} \
    \
	for (i = 0; i < sizeof(type); ++i) {\
        hashChar = hashTable[(str[0] + i) % 256];\
        for (j = 0; j < len; ++j) {\
            hashChar ^= str[j]; \
            hashChar = hashTable[hashChar]; \
        }\
        retVal <<= 8;\
        retVal |= hashChar;\
    }\
    \
    return retVal;\
}

#define HASH8_TEMPLATE(name,type) \
type Hash8_##name(pHash8 h, const char* str) { \
 \
    pHash8_Obj hash = (pHash8_Obj) h;\
	type retVal = 0; \
	uint8_t hashChar; \
	const uint8_t *hashTable = defaultHashTable; \
	size_t i, j, len=strlen(str); \
    \
    if (hash) {hashTable = hash->hashTable;}\
    \
	for (i = 0; i < sizeof(type); ++i) {\
        hashChar = hashTable[(str[0] + i) % 256];\
        for (j = 0; j < len; ++j) {\
            hashChar ^= str[j]; \
            hashChar = hashTable[hashChar];\
        }\
        retVal <<= 8;\
        retVal |= hashChar;\
    }\
    \
    return retVal;\
}


//Hash a string using a simple hash function
//  Can export to any type of object, but must be listed in instantiations
HASH8_TEMPLATE(U8,uint8_t)
HASH8_TEMPLATE(U16,uint16_t)
HASH8_TEMPLATE(U32,uint32_t)
HASH8_TEMPLATE(U64,uint64_t)

HASH8_LENGTH_TEMPLATE(U8,uint8_t)
HASH8_LENGTH_TEMPLATE(U16,uint16_t)
HASH8_LENGTH_TEMPLATE(U32,uint32_t)
HASH8_LENGTH_TEMPLATE(U64,uint64_t)
