#include <volk_sfmt/volk_sfmt.h>
#include <volk_sfmt/volk_sfmt_64f_genrand.h>
#include <iostream>
#include <cmath>

/* Here goes the logic to return a single pseudo random number. */

static inline void sfmt_genrand(double* output, double* states, uint32_t* index){
    if(*index>=DSFMT_N64){
        volk_sfmt_64f_genrand(states);
        *index = 0;
    }
    *output = states[*index]-1.0; // the -1.0 is needed because the DSFMT algorithm returns values in [1,2)
    *index += 1;
}

int main(void){
    // Allocate the states vector
    double *volk_states = (double*) volk_sfmt_malloc((DSFMT_N64+1)*2,volk_sfmt_get_alignment());

    // Init the states vector with a simple number generator
    uint32_t seed = 4357; // just a random seed, never ever use only zeros! Only zeros breaks the algorithm.
    volk_sfmt_64f_genrand_init(volk_states, seed);

    // Get the index on the generated numbers. Set on greater than DSFMT_N64 to run the SFMT algorithm on the first call!
    uint32_t index = DSFMT_N64;

    // Get some random numbers
    double result;
    std::cout << "Some pseudo random numbers from the VOLK SFMT implementations:" << std::endl;
    for(uint32_t k=0; k<20; k++){
        sfmt_genrand(&result, volk_states, &index);
        std::cout << result << std::endl;
    }

    // Clean-up
    //volk_sfmt_free(volk_states);
}
