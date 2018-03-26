#include <Perlin.h>
#include <Vector.h>
#include <Hash8.h>
#include <Rand64.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>


//The Perlin object is a big nasty one
typedef struct {
    pSize dim;              // Dimensions for this Perlin noise generator
    pRand64 rand;           // Random number generator (also for internal hashing)
    pHash8 hash;            // Used for the internal hashing
    size_t g_count;         // Count of all gradient vectors
    pVector_t *g_vects;     // List of all gradient vectors
    pVector_t *p_vects;     // Vectors for each point in the Unit n-dimensional cube (taken from list of g_vects)
    pVector_t *inside_vects;// Vectors for the points inside the unit cube
    size_t points;          // Total number of points in the cube
    double* start_coords;   // Buffer to store start coordinates (Holds the buffer)
    double* end_coords;     // Buffer to store end coordinates ( = start_coords + dimension)
    double* unit_coords;    // Coordinates inside the unit cube ( = start_coords + 2*dimensions)
    double* combined;       // Temp buffer for the coordinate in the cube ( = start_coords + 3*dimension)
    double* dot_products;   // List of dot products for each point in the cube
} Perlin_Obj_t, *pPerlin_Obj_t;

//Private internal functions
static int build_gradient_vectors(pPerlin_Obj_t perlin);
static pVector_t next_gradient_vector(pVector_t vector, pSize dim);
static inline size_t get_cube_edge_count(pSize dimension) {
    return (((size_t) dimension) * (((size_t) 1) << (dimension-1)));
}
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

    size_t i;
    if (dimensions <= 0) {return NULL;}

    pPerlin_Obj_t perlin = calloc(1,sizeof(Perlin_Obj_t));
    if (!perlin) {return NULL;}

    perlin->dim = dimensions;
    perlin->rand = New_Rand64_Seed(seed);
    if (!perlin->rand) {goto error;}

    perlin->hash = New_Hash8(perlin->rand);
    if (!perlin->hash) {goto error;}

    if (build_gradient_vectors(perlin) != 0) {goto error;}


    //Allocate the point vectors
    perlin->points = ((size_t) 1) << dimensions;
    perlin->p_vects = (pVector_t*) calloc(perlin->points, sizeof(pVector_t));
    if (!perlin->p_vects) {goto error;}


    //Allocate the inside vectors
    //   There should be one vector for each point
    perlin->inside_vects = (pVector_t*) calloc(perlin->points, sizeof(pVector_t));
    if (!perlin->inside_vects) {goto error;}
    for (i = 0; i < perlin->points; ++i) {
        perlin->inside_vects[i] = new_vector(dimensions);
        if (!perlin->inside_vects[i]) {goto error;}
    }

    //Allocate space to store the XY coordinates temporarily
    //  Only allocate one buffer
    perlin->start_coords = malloc(sizeof(double) * (dimensions * 4));
    if (!perlin->start_coords) {goto error;}
    perlin->end_coords = (perlin->start_coords + dimensions);
    perlin->unit_coords = (perlin->start_coords + (2*dimensions));
    perlin->combined = (perlin->start_coords + (3*dimensions));

    //Finally, allocate the dot product space (one per point)
    perlin->dot_products = malloc(sizeof(double) * perlin->points);
    if (!perlin->dot_products) {goto error;}

    return (pPerlin_t) perlin;

error:
    free_perlin((pPerlin_t) perlin);
    return NULL;
}


//Assumes that everything is correct going into this function
//  Returns a negative on failure, 0 on success
static int build_gradient_vectors(pPerlin_Obj_t perlin) {

    if (!perlin) {return -1;}
    if (perlin->dim <= 0) {return -2;}

    //How many vectors to allocate
    //  Special case for 1D perlin noise
    perlin->g_count = (perlin->dim == 1) ? 1 : get_cube_edge_count(perlin->dim);
    perlin->g_vects = (pVector_t*) calloc(perlin->g_count,sizeof(pVector_t));
    if (!perlin->g_vects) {return -3;}

    //Now, build all of the vectors
    pVector_t vect = NULL;
    size_t i;
    for (i = 0; i < perlin->g_count; ++i) {
        vect = next_gradient_vector(vect,perlin->dim);
        if (!vect) {return -4;}
        perlin->g_vects[i] = vect;
    }

    return 0;
}

//The gradient vector is built in sequence using the previous
// Vector as the next one in the sequence
//
//  Give it a NULL pointer to start the sequence
static pVector_t next_gradient_vector(pVector_t vector, pSize dim) {
    if (dim <= 0) {return NULL;}

    pVector_t new_vect = new_vector_values(dim,get_vector_array(vector));
    if (!new_vect) {return NULL;}
    vArr arr = get_vector_array(new_vect);

    //Start the new sequence
    if (!vector) {

        //Special case to ignore zeroes for 2 dimensions
        arr[0] = (dim < 3) ? 1 : 0;

        pSize i;
        for (i = 1; i < dim; ++i) {
            arr[i] = 1;
        }

        return new_vect;
    }

    //Continue the sequence
    pSize i, zero = dim;
    for (i = 0; i < dim; ++i) {
        if      (arr[i] == 0) {zero = i;}
        else if (arr[i] > 0) {arr[i] = -1; break;}
        else if (arr[i] < 0) {arr[i] = 1;}
    }

    //When to move the zero
    if ((dim > 2) && (i == dim)) {
        arr[zero] = 1;
        arr[(zero + 1) % dim] = 0;
    }

    return new_vect;
}



void free_perlin(pPerlin_t p) {
    if (!p) {return;}
    pPerlin_Obj_t perlin = (pPerlin_Obj_t) p;

    if (perlin->rand) {printf("Free rand64\n"); Free_Rand64(perlin->rand);}
    if (perlin->hash) {printf("Free hash8\n"); Free_Hash8(perlin->hash);}
    if (perlin->g_vects) {
        size_t i;
        for (i = 0; i < perlin->g_count; ++i) {
            if (perlin->g_vects[i]) {
                printf("Free G Vector %d\n",(int) i);
                free_vector(perlin->g_vects[i]);
            }
        }

        printf("Free G Vects\n");
        free(perlin->g_vects);
    }

    if (perlin->p_vects) {printf("Free P-Vects\n"); free(perlin->p_vects);}

    if (perlin->inside_vects) {
        size_t i;
        for (i = 0; i < perlin->points; ++i) {
            if (perlin->inside_vects[i]) {
                printf("Free inside vect %d\n",(int) i);
                free_vector(perlin->inside_vects[i]);
            }
        }

        printf("Free Inside Vects\n");
        free(perlin->inside_vects);
    }

    if (perlin->start_coords) {printf("Free Start Coords\n"); free(perlin->start_coords);}
    if (perlin->dot_products) {printf("Free Dot products\n"); free(perlin->dot_products);}

    printf("Free Perlin\n");
    free(perlin);
}


double perlin_noise(pPerlin_t p, double* coords) {

    if (!p) {return 0;}
    if (!coords) {return 0;}

    pPerlin_Obj_t perlin = (pPerlin_Obj_t) p;

    //Find the start and end coordinates of the unit cube
    size_t i, j;
    for (i = 0; i < perlin->dim; ++i) {
        perlin->start_coords[i] = floor(coords[i]);
        perlin->end_coords[i] = perlin->start_coords[i] + 1;
        perlin->unit_coords[i] = coords[i] - perlin->start_coords[i];
    }

    //Now, do vector calculations on all points
    for (i = 0; i < perlin->points; ++i) {

        //Use fancy binary maths to figure out what to copy
        // Old Points: [x,y,z]
        // New Points: [X,Y,Z]
        //
        //  If xyz = 0 and XYZ = 1, then 1<<3 = 8 -> 8 total points
        //    point[0] = point[000] = xyz
        //    point[1] = point[001] = Xyz
        //    point[2] = point[010] = xYz
        //    point[3] = point[011] = XYz
        //    ...
        //    point[7] = point[111] = XYZ

        //Get the coordinates for an individual point
        for (j = 0; j < perlin->dim; ++j) {
            if ((i >> j) & 1) {
                perlin->combined[j] = perlin->end_coords[j];
            } else {
                perlin->combined[j] = perlin->start_coords[j];
            }
        }

        // Inside vector inside the unit cube
        vArr vectarr = get_vector_array(perlin->inside_vects[i]);
        for (j = 0; j < perlin->dim; ++j) {
            vectarr[j] = (coords[j] - perlin->combined[j]);
        }

        // Outside vectors (Randomly chosen from gradient vectors list,
        //   using a quick hash algorithm)
        uint64_t toUse = Hash8_U64_Length(perlin->hash,(char*) perlin->combined, ((size_t) perlin->dim) * sizeof(double));
        perlin->p_vects[i] = perlin->g_vects[toUse % perlin->g_count];

        // Dot product of the two vectors
        //  These values fill our "Dot Product" array
        perlin->dot_products[i] = vector_dot_product(perlin->p_vects[i],perlin->inside_vects[i]);
    }



    //With all of the dot products calculated, it is time to do a MEGA MERGE:
    //  Once again, this works using binary maths:
    //  [0,1,2,3,4,5,6,7]
    //   \_/ \_/ \_/ \_/   (Interpolate with unit X, space of 2)
    //    [0, 2,  4,  6]
    //     \__/   \___/    (Interpolate with unit Y, space of 4)
    //       [0,    4]
    //        \_____/      (Interpolate with unit Z, space of 8)
    //     (Final Value)
    for (i = 0; i < perlin->dim; ++i) {
        size_t incrament = ((size_t) 1) << i;
        for (j = 0; j < perlin->points; j+= (incrament *2)) {
            perlin->dot_products[j] =
                interpolate(perlin->dot_products[j],perlin->dot_products[j+incrament],
                            smooth(perlin->unit_coords[i]));
        }
    }

    //Limit values from 0 to 1 (We get, by default, -1 to 1)
    return (perlin->dot_products[0] + 1) /2;
}
