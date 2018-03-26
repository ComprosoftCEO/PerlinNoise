#ifndef HASH_8_HEADER
#define HASH_8_HEADER

#include <stdlib.h>
#include <stdint.h>
#include <Rand64.h>


typedef void* pHash8;

pHash8 New_Hash8(pRand64 rand);
void Hash8_Reseed(pHash8, pRand64);
void Free_Hash8(pHash8);

//Passing a NULL pHash8 object uses the default Hash8 table
#define HASH8_FUNCTION(name,type) type Hash8_##name(pHash8, const char* str)
#define HASH8_LENGTH_FUNCTION(name,type) type Hash8_##name##_Length(pHash8, const char* str, size_t len)

HASH8_FUNCTION(U8,uint8_t);
HASH8_FUNCTION(U16,uint16_t);
HASH8_FUNCTION(U32,uint32_t);
HASH8_FUNCTION(U64,uint64_t);

HASH8_LENGTH_FUNCTION(U8,uint8_t);
HASH8_LENGTH_FUNCTION(U16,uint16_t);
HASH8_LENGTH_FUNCTION(U32,uint32_t);
HASH8_LENGTH_FUNCTION(U64,uint64_t);


#endif // HASH_8_HEADER Included
