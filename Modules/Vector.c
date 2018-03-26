#include <Vector.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//Private vector object type:
typedef struct {
    vSize dim;      // Dimensions of the structure
    vArr  data;     // Internal buffer that stores the data
} Vector_Obj_t, *pVector_Obj_t;


pVector_t new_vector(vSize dimension) {

    //Test for an invalid dimension given
    if (dimension <= 0) {return NULL;}

    pVector_Obj_t vector = (pVector_Obj_t) malloc(sizeof(Vector_Obj_t));
    if (!vector) {return NULL;}

    //Initial vector should be filled with 0's
    vector->dim = dimension;
    vector->data = (vArr) calloc(dimension, sizeof(vType));
    if (!vector->data) {
        free_vector((pVector_t) vector);
        return NULL;
    }

    return (pVector_t) vector;
}


void free_vector(pVector_t v) {
    if (!v) {return;}
    pVector_Obj_t vector = (pVector_Obj_t) v;

    if (vector->data) {free(vector->data);}
    free(vector);
}


pVector_t new_vector_values(vSize dimension,vArr data) {
    pVector_t vector = new_vector(dimension);

    if (!vector) {return NULL;}
    set_vector_values(vector,data);

    return vector;
}




void print_vector(pVector_t v) {
    if (!v) {goto error;}
    pVector_Obj_t vector = (pVector_Obj_t) v;
    if (!vector->data) {goto error;}

    vIndex i;
    printf("{");
    for (i = 0; i < vector->dim; ++i) {
        if (i > 0) {printf(",");}
        printf("%f",vector->data[i]);
    }
    printf("}\n");
    return;

error:
    printf("<NULL>\n");
}



void set_vector_value(pVector_t v, vIndex index, vType value) {
    if (!v) {return;}
    pVector_Obj_t vector = (pVector_Obj_t) v;
    if (!vector->data) {return;}

    if (index >= vector->dim) {return;}
    vector->data[index] = value;
}

void set_vector_values(pVector_t v, vArr data) {
    if (!v) {return;}
    if (!data) {return;}

    pVector_Obj_t vector = (pVector_Obj_t) v;
    if (!vector->data) {return;}

    vIndex i;
    for (i = 0; i < vector->dim; ++i) {
        vector->data[i] = data[i];
    }
}

vType get_vector_value(pVector_t v, vIndex index) {
    if (!v) {return 0;}
    pVector_Obj_t vector = (pVector_Obj_t) v;
    if (!vector->data) {return 0;}

    if (index >= vector->dim) {return 0;}
    return vector->data[index];
}


vArr get_vector_array(pVector_t v) {
    if (!v) {return 0;}
    return ((pVector_Obj_t) v)->data;
}

vSize get_vector_dimensions(pVector_t v) {
   if (!v) {return 0;}
    return ((pVector_Obj_t) v)->dim;
}






pVector_t redim_vector(pVector_t v, vSize new_dim) {
    if (!v) {return NULL;}
    if (new_dim <= 0) {return NULL;}
    pVector_Obj_t vector = (pVector_Obj_t) v;

    vArr new_data;
    if (!vector->data) {
        new_data = (vArr) calloc(new_dim,sizeof(vType));
    } else {
        new_data = (vArr) realloc(vector->data,sizeof(vType) * new_dim);
    }

    if (!new_data) {return NULL;}
    vector->data = new_data;
    vector->dim = new_dim;

    return v;
}


pVector_t redim_vector_values(pVector_t v, vSize new_dim, vArr data) {
    if (redim_vector(v,new_dim)) {
        set_vector_values(v,data);
        return v;
    }

    return NULL;
}


vType vector_dot_product(pVector_t v1, pVector_t v2) {
    if ((!v1) || (!v2)) {return 0;}
    pVector_Obj_t vector1, vector2;

    vector1 = (pVector_Obj_t) v1;
    vector2 = (pVector_Obj_t) v2;

    if (vector1->dim != vector2->dim) {return 0;}

    vType ret = 0;
    vIndex i, dim = vector1->dim;
    for (i = 0; i < dim; ++i) {
        ret += (vector1->data[i] * vector2->data[i]);
    }

    return ret;
}
