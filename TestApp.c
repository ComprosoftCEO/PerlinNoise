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
        for (j = 0; j < 25; ++j) {
            random_seed_perlin(perlin);

            double coords[2] = {-0.2,10.4};
            double pre = perlin_noise_octave(perlin,(double[]) {-0.2, 10.4},4,0.5);
            double cur = perlin_noise_octave(perlin,(double[]) {-0.1, 10.4},4,0.5);
            double new = perlin_noise_octave(perlin,(double[]) {0.0,  10.4},4,0.5);

            if (cur > pre && cur > new ||
                cur < pre && cur < new) {
                    printf("%f %f %f\n",pre,cur,new);
                }


        }
        //}
        free_perlin(perlin);
        free_legacy_perlin(lperlin);
    } else {
        printf("Failure to allocate!\n");
    }

    return 0;
}
