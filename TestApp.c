#include <Perlin.h>
#include <Perlin-Legacy.h>
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
    pLegacy_Perlin_t lperlin = new_legacy_perlin(2);
    if (perlin && lperlin) {

        int i, j;
        for (i = 0; i < 100; ++i) {
        for (j = 0; j < 100; ++j) {

            double coords[2] = {0};
            coords[0] = ((double)i) / 100;
            coords[1] = ((double)j) / 100;

            printf("<%f,%f>\n",
                   perlin_noise(perlin,coords),
                   legacy_perlin_noise(lperlin,coords));
        }}
        free_perlin(perlin);
        free_legacy_perlin(lperlin);
    } else {
        printf("Failure to allocate!\n");
    }

    return 0;
}
