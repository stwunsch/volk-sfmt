#include <volk_sfmt/volk_sfmt.h>
#include <volk_sfmt/volk_sfmt_32u_genrand.h>
#include <iostream>
#include <cmath>

/* Here goes the logic to return a single pseudo random number. */

static inline void sfmt_genrand(uint32_t* output, uint32_t* states, uint32_t* index){
    if(*index>=SFMT_N32){
        volk_sfmt_32u_genrand(states);
        *index = 0;
    }
    *output = states[*index];
    *index += 1;
}

#define ENLARGE_BUFFER 10 // this sets a ten time increased buffer compared to the conventional SFMT implementation

static inline void sfmt_genrand_improved(uint32_t* output, uint32_t* buffer, uint32_t* states, uint32_t* index){
    if(*index>=SFMT_N32*ENLARGE_BUFFER){
        volk_sfmt_32u_genrand_array_32u(buffer,states,ENLARGE_BUFFER*SFMT_N32);
        *index = 0;
    }
    *output = buffer[*index];
    *index += 1;
}

/* This is a simple (and as well 'bad' for most uses) pseudo number generator to initialize the states of the SFMT. */

static inline void sfmt_genrand_init(uint32_t *states, uint32_t seed){
    int i;
    states[0] = seed;
    for (i = 1; i < SFMT_N32; i++) {
        states[i] = 1812433253UL * (states[i - 1] ^ (states[i - 1] >> 30)) + i;
    }
}

int main(void){
    // Allocate the states vector
    uint32_t *volk_states = (uint32_t*) volk_sfmt_malloc(SFMT_N32*4,volk_sfmt_get_alignment());
    uint32_t *volk_states_array = (uint32_t*) volk_sfmt_malloc(SFMT_N32*4,volk_sfmt_get_alignment());
    uint32_t *buffer_array = (uint32_t*) volk_sfmt_malloc(SFMT_N32*4*ENLARGE_BUFFER,volk_sfmt_get_alignment()); // volk impl

    // Init the states vector with a simple number generator
    uint32_t seed = 4357; // just a random seed, never ever use only zeros! Only zeros breaks the algorithm.
    sfmt_genrand_init(volk_states, seed);
    sfmt_genrand_init(volk_states_array, seed);

    // Get the index on the generated numbers. Set on greater than SFMT_N32==624 to run the SFMT algorithm on the first call!
    uint32_t index = std::pow(2,31);
    uint32_t index_array = std::pow(2,31);

    // Get some random numbers
    uint32_t result, result_array;
    std::cout << "Some pseudo random numbers from the VOLK SFMT implementations:" << std::endl;
    for(uint32_t k=0; k<10; k++){
        sfmt_genrand(&result, volk_states, &index);
        sfmt_genrand_improved(&result_array, buffer_array, volk_states_array, &index_array);
        std::cout << result << " " << result_array << std::endl;
    }

    // Clean-up
    volk_sfmt_free(volk_states);
    volk_sfmt_free(volk_states_array);
    volk_sfmt_free(buffer_array);
}
