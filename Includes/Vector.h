#ifndef VECTOR_HEADER
#define VECTOR_HEADER

#include <stdint.h>
typedef void* pVector_t;

//Internal type for a vector: (May change later)
//  Also define an array, and a macro to easily build arrays
typedef double vType;
typedef vType* vArr;
#define V_ARR(...) (vType[]) {__VA_ARGS__}

//Internal type for a vector size (and index)
typedef uint16_t vSize;
typedef vSize vIndex;


pVector_t new_vector(vSize dimension);
void free_vector(pVector_t vector);

//Define a vector with [dimension] number of points
//  Each point is a vtype
//  Be sure that the count of vArr and dimensions matah
pVector_t new_vector_values(vSize dimension,vArr data);

void print_vector(pVector_t vector);

//Getters and setters for the internal array type
void set_vector_value(pVector_t vector, vIndex index, vType value);
void set_vector_values(pVector_t vector, vArr data);
vType get_vector_value(pVector_t vector, vIndex index);

vArr get_vector_array(pVector_t);
vSize get_vector_dimensions(pVector_t);

//Resize the dimensions of the vector
//  Return a pointer to the same object, but used for error checking if NULL
pVector_t redim_vector(pVector_t vector, vSize new_dim);
pVector_t redim_vector_values(pVector_t vector, vSize new_dim, vArr data);


//Other vector math functions
vType vector_dot_product(pVector_t one, pVector_t two);


#endif // VECTOR_HEADER Included
