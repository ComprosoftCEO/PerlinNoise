#include <Perlin.h>
#include <Vector.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static inline uint64_t get_cube_edge_count(uint16_t dimension) {
    return (dimension * (1 << (dimension-1)));
}


//Testing utility
int main() {

    pPerlin_t perlin = new_perlin(2);
    if (perlin) {

        int i, j;
        for (i = 0; i < 100; ++i) {
        for (j = 0; j < 100; ++j) {

            double coords[2];
            coords[0] = ((double) i) / 100;
            coords[1] = ((double) j) / 100;

            printf("%f\n",perlin_noise(perlin, coords));
        }}
        free_perlin(perlin);
    } else {
        printf("Failure!\n");
    }

    return 0;
}
