//The newer implementation of Perlin Noise
//
//  Much better at handling higher dimensional noise
//   without needing more space to store it
//
//  Also gets way slower...
#include <Perlin.h>
#include <Vector.h>
#include <Hash8.h>
#include <Rand64.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>


//The Perlin object is a big nasty one
typedef struct {
    pSize dim;              // Dimensions for this Perlin noise generator
    pRand64 rand;           // Random number generator (also for internal hashing)
    pHash8 hash;            // Used for the internal hashing
    uint8_t* counter;       // Used for iterating over every point in the Unit Cube (dim + 1)
    double* start_coords;   // Buffer to store start coordinates (Also holds the buffer for end, unit, and dot)
    double* end_coords;     // Buffer to store end coordinates
    double* pnt_bldr[2];    // Point Builder - Used to do fast copy when merging start & end coordinates into a point coordinate
    double* unit_coords;    // XY coordinates inside the unit cube (0.0 to 1.0), modified by fade before coordinates are copied
    double* dot_products;   // Dot products buffer for computing as the algorithm runs (dot_product[dim] = Final Value)
    pVector_t g_vect;       // Temporary gradient vector
    pVector_t p_vect;       // Temporary point vector (vector between point on the cube and coordiantes inside the cube)
} Perlin_Obj_t, *pPerlin_Obj_t;


static inline double interpolate(double one, double two, double amount) {
    return ((one * (1 - amount)) + (two * amount));
}
static inline double smooth(double input) {
 return input * input * input * (input * ((input * 6) - 15) + 10);
}


pPerlin_t new_perlin(pSize dimensions) {
    static pRand64 rand = NULL;
    if (rand == NULL) {rand = New_Rand64();}
    return new_perlin_seed(dimensions,Rand64_Next(rand));
}


pPerlin_t new_perlin_seed(pSize dimensions, uint64_t seed) {

    if (dimensions <= 0) {return NULL;}

    pPerlin_Obj_t perlin = calloc(1,sizeof(Perlin_Obj_t));
    if (!perlin) {return NULL;}

    //Rand64 and Hash8 for finding gradient vectors
    perlin->dim = dimensions;
    perlin->rand = New_Rand64_Seed(seed);
    if (!perlin->rand) {goto error;}
    perlin->hash = New_Hash8(perlin->rand);
    if (!perlin->hash) {goto error;}

    //Counter
    // Has dimension + 1 spaces to test for overflow
    // When overflow spot = 1, then stop the loop
    perlin->counter = malloc(sizeof(uint8_t) * (dimensions + 1));
    if (!perlin->counter) {goto error;}


    //Internal buffers for Unit Start, Unit End, and Unit Coordinates
    //  Allocates one space at the end for the final dot product
    //  Which is based on the coutner index (which overflows to dim + 1)
    perlin->start_coords = malloc(sizeof(double) * ((dimensions*4) + 1));
    if (!perlin->start_coords) {goto error;}
    perlin->end_coords = (perlin->start_coords + (dimensions));
    perlin->unit_coords = (perlin->start_coords + (dimensions*2));
    perlin->dot_products = (perlin->start_coords + (dimensions*3));
    perlin->pnt_bldr[0] = perlin->start_coords;
    perlin->pnt_bldr[1] = perlin->end_coords;

    //Allocate the vectors that are used for dot products
    perlin->g_vect = new_vector(dimensions);
    if (!perlin->g_vect) {goto error;}
    perlin->p_vect = new_vector(dimensions);
    if (!perlin->p_vect) {goto error;}

    return (pPerlin_t) perlin;

error:
    free_perlin((pPerlin_t) perlin);
    return NULL;
}




void free_perlin(pPerlin_t p) {
    if (!p) {return;}
    pPerlin_Obj_t perlin = (pPerlin_Obj_t) p;

    if (perlin->rand) {Free_Rand64(perlin->rand);}
    if (perlin->hash) {Free_Hash8(perlin->hash);}
    if (perlin->counter) {free(perlin->counter);}
    if (perlin->start_coords) {free(perlin->start_coords);}
    if (perlin->g_vect) {free_vector(perlin->g_vect);}
    if (perlin->p_vect) {free_vector(perlin->p_vect);}
    free(perlin);
}






//Return a value 0.0 to 1.0 for a given point in N-Dimensional space
double perlin_noise(pPerlin_t p, double* coords) {

    if (!p) {return 0;}
    if (!coords) {return 0;}

    pPerlin_Obj_t perlin = (pPerlin_Obj_t) p;
    double *g_arr = get_vector_array(perlin->g_vect);
    double *p_arr = get_vector_array(perlin->p_vect);
    size_t p_arr_len = ((size_t) perlin->dim) * sizeof(double);
    size_t i;


    //Prepare the coordinates
    for (i = 0; i < perlin->dim; ++i) {
        perlin->start_coords[i] = floor(coords[i]);
        perlin->end_coords[i] = perlin->start_coords[i] + 1;
        perlin->unit_coords[i] = smooth(coords[i] - perlin->start_coords[i]);
        p_arr[i] = coords[i] - perlin->start_coords[i];
    }


    //Reset the counter, and loop until it overflows
    uint8_t *trigger = (perlin->counter + perlin->dim);
    memset(perlin->counter,0, (perlin->dim + 1) * sizeof(uint8_t));
    while(*trigger == 0) {

        //Use an algorithm to generate the unique gradient vector
        //
        //  1. Fill every spot with a number (-1 or 1)
        //  2. For dimensions > 2, pick a spot to store 0
        uint64_t hash = Hash8_U64_Length(perlin->hash,(const char*) p_arr, p_arr_len);
        Rand64_Reseed(perlin->rand,hash);
        for (i = 0; i < perlin->dim; ++i) {
            g_arr[i] = (Rand64_Next(perlin->rand) & 1) ? 1 : -1;
        }
        if (perlin->dim > 2) {
            g_arr[Rand64_Next(perlin->rand) % perlin->dim] = 0;
        }


        //Find the dot product, then propagate along the "chain"
        //  Until the final value is reached
        //
        //  The counter is used as a type of "Key"
        //      When 0, then store in the spot
        //      When 1, then interpolate and propagate to the next spot
        double dot = vector_dot_product(perlin->g_vect, perlin->p_vect);
        i = 0;
    merge:
        if (perlin->counter[i]!= 0) {
            dot = interpolate(perlin->dot_products[i],dot,perlin->unit_coords[i]);
            ++i;
            goto merge;
        } else {
            perlin->dot_products[i] = dot;
        }


        //Advance the counter
        //  Also, update the Point Coordinates while the counter is running
        //  This algorithm only updates a point when it changes...
        i = 0;
    advance:
        perlin->counter[i] ^= 1;
        if (i < perlin->dim) {    //Avoid buffer overflow
            p_arr[i] = coords[i] - perlin->pnt_bldr[perlin->counter[i]][i];
        }
        if (perlin->counter[i] == 0) {
            ++i;
            goto advance;
        }
    }

    return (perlin->dot_products[perlin->dim] + 1) / 2;
}
