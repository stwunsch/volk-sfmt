/*** DEPENDENCIES ***
 * boost.random
 * VOLK SFMT module
 * C++11
 */

/*** NOTE ***
 * You need a lot of memory for this (about 8GB)!
 * Reduce 'num_gen' if you run into truble.
 */

#include <volk_sfmt/volk_sfmt.h>
#include "../reference_impl/SFMT.h"
#include <boost/random.hpp>
#include <chrono>

using namespace std::chrono;

static inline void sfmt_genrand_sse2(uint32_t* output, uint32_t* states, uint32_t* index){
    if(*index>=SFMT_N32){
        volk_sfmt_32u_genrand_manual(states,"a_sse2");
        *index = 0;
    }
    *output = states[*index];
    *index += 1;
}

static inline void sfmt_genrand_generic(uint32_t* output, uint32_t* states, uint32_t* index){
    if(*index>=SFMT_N32){
        volk_sfmt_32u_genrand_manual(states,"generic");
        *index = 0;
    }
    *output = states[*index];
    *index += 1;
}

static inline void sfmt_genrand_init(uint32_t *states, uint32_t seed){
    int i;
    states[0] = seed;
    for (i = 1; i < SFMT_N32; i++) {
        states[i] = 1812433253UL * (states[i - 1] ^ (states[i - 1] >> 30)) + i;
    }
}

#if defined(HAVE_SSE2)
void run_mode(){
    std::cout << "The original implementation uses SSE2." << std::endl;
}
#endif

#if !defined(HAVE_SSE2)
void run_mode(){
    std::cout << "The original implementation DOES NOT use SSE2." << std::endl;
}
#endif

static inline unsigned int mt_rndm_uint32(unsigned int *state, unsigned int *count624){
    unsigned int y;

    const unsigned int M = 397;
    const unsigned int N = 624;
    const unsigned int upperMask = 0x80000000;
    const unsigned int lowerMask = 0x7fffffff;
    const unsigned int temperingMaskA = 0x9d2c5680;
    const unsigned int temperingMaskB = 0xefc60000;
    const unsigned int matrixA = 0x9908b0df;

    // If all of the N numbers are used, generate N new ones.
    if(*count624>=N){
        // The actual random number generator comes here!
        unsigned int k;

        for(k=0; k<N-M; k++){
            y = (state[k] & upperMask) | (state[k+1] & lowerMask);
            state[k] = state[k+M] ^ (y >> 1) ^ ((y & 0x1) ? matrixA : 0x0);
        }

        for(; k<N-1; k++){
            y = (state[k] & upperMask) | (state[k+1] & lowerMask);
            state[k] = state[k+M-N] ^ (y >> 1) ^ ((y & 0x1) ? matrixA : 0x0);
        }

        y = (state[N-1] & upperMask) | (state[0] & lowerMask);
        state[N-1] = state[M-1] ^ (y >> 1) ^ ((y & 0x1) ? matrixA : 0x0);

        *count624 = 0;
    }

    // Perform tempering on generated numbers.
    y = state[*count624];
    *count624 = *count624+1;
    y ^= y >> 11;
    y ^= ((y << 7) & temperingMaskA);
    y ^= ((y << 15) & temperingMaskB);
    y ^= y >> 18;

    return y;
}

int main(void){
    /* SETUP */
    uint32_t num_gen = SFMT_N32*100000; // test num_gen/SFMT_N32 cycles of the SFMT algorithm
    run_mode(); // get mode of original implementation (sse2 or generic)
    high_resolution_clock::time_point t1, t2;
    size_t alignment = volk_sfmt_get_alignment(); // get system memory alignment
    uint32_t *init_states = (uint32_t*) volk_sfmt_malloc(SFMT_N32*4,alignment);
    sfmt_genrand_init(init_states, 4357);

    /* Runtime: SFMT (VOLK implementation, generic) */
    uint32_t runtime_volk_generic;
    uint32_t *volk_generic_states = (uint32_t*) volk_sfmt_malloc(SFMT_N32*4,alignment);
    uint32_t *volk_generic_results = (uint32_t*) volk_sfmt_malloc(num_gen*4,alignment);
    for(uint32_t k=0; k<SFMT_N32; k++) volk_generic_states[k] = init_states[k];
    uint32_t volk_generic_index = SFMT_N32; // this produces new random numbers on the first call of the SFMT
    t1 = high_resolution_clock::now();
    for(uint32_t k=0; k<num_gen; k++) sfmt_genrand_generic(volk_generic_results+k,volk_generic_states,&volk_generic_index);
    t2 = high_resolution_clock::now();
    runtime_volk_generic = duration_cast<microseconds>(t2-t1).count();

    /* Runtime: SFMT (VOLK implementation, generic, array) */
    uint32_t runtime_volk_array_generic;
    uint32_t *volk_array_generic_states = (uint32_t*) volk_sfmt_malloc(SFMT_N32*4,alignment);
    uint32_t *volk_array_generic_results = (uint32_t*) volk_sfmt_malloc(num_gen*4,alignment);
    for(uint32_t k=0; k<SFMT_N32; k++) volk_array_generic_states[k] = init_states[k];
    t1 = high_resolution_clock::now();
    volk_sfmt_32u_genrand_array_32u_manual(volk_array_generic_results, volk_array_generic_states, num_gen, "generic");
    t2 = high_resolution_clock::now();
    runtime_volk_array_generic = duration_cast<microseconds>(t2-t1).count();

    /* Runtime: SFMT (VOLK implementation, sse2) */
    uint32_t runtime_volk_sse2;
    uint32_t *volk_sse2_states = (uint32_t*) volk_sfmt_malloc(SFMT_N32*4,alignment);
    uint32_t *volk_sse2_results = (uint32_t*) volk_sfmt_malloc(num_gen*4,alignment);
    for(uint32_t k=0; k<SFMT_N32; k++) volk_sse2_states[k] = init_states[k];
    uint32_t volk_sse2_index = SFMT_N32; // this produces new random numbers on the first call of the SFMT
    t1 = high_resolution_clock::now();
    for(uint32_t k=0; k<num_gen; k++) sfmt_genrand_sse2(volk_sse2_results+k,volk_sse2_states,&volk_sse2_index);
    t2 = high_resolution_clock::now();
    runtime_volk_sse2 = duration_cast<microseconds>(t2-t1).count();

    /* Runtime: SFMT (VOLK implementation, sse2, array) */
    uint32_t runtime_volk_array_sse2;
    uint32_t *volk_array_sse2_states = (uint32_t*) volk_sfmt_malloc(SFMT_N32*4,alignment);
    uint32_t *volk_array_sse2_results = (uint32_t*) volk_sfmt_malloc(num_gen*4,alignment);
    for(uint32_t k=0; k<SFMT_N32; k++) volk_array_sse2_states[k] = init_states[k];
    t1 = high_resolution_clock::now();
    volk_sfmt_32u_genrand_array_32u_manual(volk_array_sse2_results, volk_array_sse2_states, num_gen, "a_sse2");
    t2 = high_resolution_clock::now();
    runtime_volk_array_sse2 = duration_cast<microseconds>(t2-t1).count();

    /* Runtime: SFMT (original implementation) */
    uint32_t runtime_sfmt_orig;
    sfmt_t *sfmt = new sfmt_t;
    uint32_t *sfmt_states = (uint32_t*) sfmt->state;
    for(uint32_t k=0; k<SFMT_N32; k++) sfmt_states[k] = init_states[k];
    sfmt->idx = SFMT_N32;
    uint32_t *sfmt_orig_results = (uint32_t*) volk_sfmt_malloc(num_gen*4,alignment);
    t1 = high_resolution_clock::now();
    for(uint32_t k=0; k<num_gen; k++) sfmt_orig_results[k] = sfmt_genrand_uint32(sfmt);
    t2 = high_resolution_clock::now();
    runtime_sfmt_orig = duration_cast<microseconds>(t2-t1).count();
    delete sfmt;

    /* Runtime: SFMT (original implementation) */
    uint32_t runtime_sfmt_array_orig;
    sfmt_t *sfmt_array = new sfmt_t;
    uint32_t *sfmt_array_states = (uint32_t*) sfmt_array->state;
    for(uint32_t k=0; k<SFMT_N32; k++) sfmt_array_states[k] = init_states[k];
    sfmt_array->idx = SFMT_N32;
    uint32_t *sfmt_array_orig_results = (uint32_t*) volk_sfmt_malloc(num_gen*4,alignment);
    t1 = high_resolution_clock::now();
    sfmt_fill_array32(sfmt_array,sfmt_array_orig_results,num_gen);
    t2 = high_resolution_clock::now();
    runtime_sfmt_array_orig = duration_cast<microseconds>(t2-t1).count();
    delete sfmt_array;

    /* Runtime: Standard Mersenne-Twister */
    uint32_t runtime_mt_std;
    uint32_t *mt_std_states = (uint32_t*) volk_sfmt_malloc(SFMT_N32*4,alignment);
    uint32_t *mt_std_results = (uint32_t*) volk_sfmt_malloc(num_gen*4,alignment);
    for(uint32_t k=0; k<SFMT_N32; k++) mt_std_states[k] = init_states[k];
    uint32_t mt_std_index = SFMT_N32; // this produces new random numbers on the first call of the SFMT
    t1 = high_resolution_clock::now();
    for(uint32_t k=0; k<num_gen; k++) mt_std_results[k] = mt_rndm_uint32(mt_std_states, &mt_std_index);
    t2 = high_resolution_clock::now();
    runtime_mt_std = duration_cast<microseconds>(t2-t1).count();

    /* Runtime: Boost.Random Mersenne-Twister */
    // I can't init the boost Mersenne-Twister manually! So I can't check whether the results are the same than the given implementations.
    uint32_t runtime_mt_boost;
    boost::mt19937 *rng = new boost::mt19937;
    boost::random::uniform_int_distribution<> *integer_dis = new boost::random::uniform_int_distribution<>();
    boost::variate_generator<boost::mt19937&, boost::random::uniform_int_distribution<> > *integer_generator;
    integer_generator = new boost::variate_generator<boost::mt19937&, boost::random::uniform_int_distribution<> >(*rng, *integer_dis);
    uint32_t *mt_boost_results = (uint32_t*) volk_sfmt_malloc(num_gen*4,alignment);
    t1 = high_resolution_clock::now();
    for(uint32_t k=0; k<num_gen; k++) mt_boost_results[k] = (*integer_generator)();
    t2 = high_resolution_clock::now();
    runtime_mt_boost = duration_cast<microseconds>(t2-t1).count();

    /* Test results of VOLK generic, VOLK SSE2 and SFMT original */
    bool all_same = true;
    for(uint32_t k=0; k<num_gen; k++){
        if(not(sfmt_orig_results[k]==volk_generic_results[k])) all_same = false;
        if(not(sfmt_orig_results[k]==volk_sse2_results[k])) all_same = false;
        if(not(sfmt_orig_results[k]==sfmt_array_orig_results[k])) all_same = false;
        if(not(sfmt_orig_results[k]==volk_array_generic_results[k])) all_same = false;
        if(not(sfmt_orig_results[k]==volk_array_sse2_results[k])) all_same = false;
    }
    if (all_same) std::cout << "VOLK generic, VOLK SSE2 and SFMT original implementations (normal and array) return the same values." << std::endl;
    else std::cout << "VOLK generic, VOLK SSE2 and SFMT original implementations (normal and array) DOES NOT return the same values." << std::endl;

    /* Print runtimes */
    std::cout << "RUNTIME in ms for " << num_gen << " numbers generated:" << std::endl;
    std::cout << "    Standard Mersenne-Twister: " << runtime_mt_std << std::endl;
    std::cout << "    Boost.Random Mersenne-Twister: " << runtime_mt_boost << std::endl;
    std::cout << "    Original impl (SFMT): " << runtime_sfmt_orig << std::endl;
    std::cout << "    Original array impl (SFMT): " << runtime_sfmt_array_orig << std::endl;
    std::cout << "    VOLK SFMT generic: " << runtime_volk_generic << std::endl;
    std::cout << "    VOLK SFMT array generic: " << runtime_volk_array_generic << std::endl;
    std::cout << "    VOLK SFMT SSE2: " << runtime_volk_sse2 << std::endl;
    std::cout << "    VOLK SFMT array SSE2: " << runtime_volk_array_sse2 << std::endl;

    std::cout << "RUNTIME relative to standard Mersenne Twister for " << num_gen << " numbers generated:" << std::endl;
    uint32_t runtime_comp = runtime_mt_std;
    std::cout << "    Standard Mersenne-Twister: " << float(runtime_comp)/runtime_mt_std << std::endl;
    std::cout << "    Boost.Random Mersenne-Twister: " << float(runtime_comp)/runtime_mt_boost << std::endl;
    std::cout << "    Original impl: " << float(runtime_comp)/runtime_sfmt_orig << std::endl;
    std::cout << "    Original array impl: " << float(runtime_comp)/runtime_sfmt_array_orig << std::endl;
    std::cout << "    VOLK generic: " << float(runtime_comp)/runtime_volk_generic << std::endl;
    std::cout << "    VOLK array generic: " << float(runtime_comp)/runtime_volk_array_generic << std::endl;
    std::cout << "    VOLK SSE2: " << float(runtime_comp)/runtime_volk_sse2 << std::endl;
    std::cout << "    VOLK array SSE2: " << float(runtime_comp)/runtime_volk_array_sse2 << std::endl;
}
