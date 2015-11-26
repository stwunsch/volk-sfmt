/*** DEPENDENCIES ***
 * boost.random
 * VOLK SFMT module
 * C++11
 */

#include <volk_sfmt/volk_sfmt.h>
#include <volk/volk.h>
#include "../dsfmt_reference_impl/dSFMT.h"
#include <boost/random.hpp>
#include <chrono>

using namespace std::chrono;

#if defined(HAVE_SSE2)
int run_mode(){
    std::cout << "The original implementation uses SSE2." << std::endl;
    return 1;
}
#endif

#if !defined(HAVE_SSE2)
int run_mode(){
    std::cout << "The original implementation DOES NOT use SSE2." << std::endl;
    return 0;
}
#endif

static inline void volk_genrand_generic(double* output, double* states, uint32_t* index){
    if(*index>=DSFMT_N64){
        volk_sfmt_64f_genrand_manual(states, "generic");
        *index = 0;
    }
    *output = states[*index]-1.0;
    *index += 1;
}

static inline void volk_genrand_sse(double* output, double* states, uint32_t* index){
    if(*index>=DSFMT_N64){
        volk_sfmt_64f_genrand_manual(states, "a_sse2");
        *index = 0;
    }
    *output = states[*index]-1.0;
    *index += 1;
}

static inline void volk_genrand_conv_generic(double* output, uint32_t* states, uint32_t* index){
    if(*index>=624){
        volk_sfmt_32u_genrand_manual(states, "generic");
        *index = 0;
    }
    *output = states[*index]/(double)(0xFFFFFFFF-0x1);
    *index += 1;
}

static inline void volk_genrand_conv_sse(double* output, uint32_t* states, uint32_t* index){
    if(*index>=624){
        volk_sfmt_32u_genrand_manual(states, "a_sse2");
        *index = 0;
    }
    *output = states[*index]/(double)(0xFFFFFFFF-0x1);
    *index += 1;
}

static inline double mt_rndm_uint32(unsigned int *state, unsigned int *count624){
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

    return y/(double)(0xFFFFFFFF-0x1);
}

int main(void){
    /* SETUP */
    int mode_original = run_mode();

    /* INIT */
    size_t alignment = volk_sfmt_get_alignment();
    std::cout << "Alignment: " << alignment << std::endl;
    double *volk_generic_states = (double*) volk_sfmt_malloc((DSFMT_N+1)*16, alignment);
    double *volk_sse_states = (double*) volk_sfmt_malloc((DSFMT_N+1)*16, alignment);
    uint32_t *volk_conv_generic_states = (uint32_t*) volk_sfmt_malloc(624*4, alignment);
    uint32_t *volk_conv_sse_states = (uint32_t*) volk_sfmt_malloc(624*4, alignment);
    uint32_t *mt_std_states = (uint32_t*) volk_sfmt_malloc(624*4,alignment);
    uint32_t seed = 4357;
    volk_sfmt_64f_genrand_init(volk_generic_states, seed);
    volk_sfmt_64f_genrand_init(volk_sse_states, seed);
    volk_sfmt_32u_genrand_init(volk_conv_generic_states, seed);
    volk_sfmt_32u_genrand_init(volk_conv_sse_states, seed);
    volk_sfmt_32u_genrand_init(mt_std_states, seed);

    dsfmt_t dsfmt;
    dsfmt_chk_init_gen_rand(&dsfmt, seed, 19937);

    uint32_t num_gen = DSFMT_N64 * 300;
    high_resolution_clock::time_point t1, t2;

    /* RUN VOLK GENERIC */
    double volk_generic_value[num_gen];
    t1 = high_resolution_clock::now();
    uint32_t volk_generic_index = DSFMT_N64;
    for(uint32_t k=0; k<num_gen; k++){
        volk_genrand_generic(volk_generic_value+k, volk_generic_states, &volk_generic_index);
    }
    t2 = high_resolution_clock::now();
    uint32_t runtime_volk_generic = duration_cast<microseconds>(t2-t1).count();

    /* RUN VOLK SSE2 */
    double volk_sse_value[num_gen];
    t1 = high_resolution_clock::now();
    uint32_t volk_sse_index = DSFMT_N64;
    for(uint32_t k=0; k<num_gen; k++){
        volk_genrand_sse(volk_sse_value+k, volk_sse_states, &volk_sse_index);
    }
    t2 = high_resolution_clock::now();
    uint32_t runtime_volk_sse = duration_cast<microseconds>(t2-t1).count();

    /* RUN VOLK GENERIC WITH CONVERSION FROM UINT32_T*/
    double volk_conv_generic_value[num_gen];
    t1 = high_resolution_clock::now();
    uint32_t volk_conv_generic_index = 624;
    for(uint32_t k=0; k<num_gen; k++){
        volk_genrand_conv_generic(volk_conv_generic_value+k, volk_conv_generic_states, &volk_conv_generic_index);
    }
    t2 = high_resolution_clock::now();
    uint32_t runtime_volk_conv_generic = duration_cast<microseconds>(t2-t1).count();

    /* RUN VOLK SSE2 WITH CONVERSION FROM UINT32_T*/
    double volk_conv_sse_value[num_gen];
    t1 = high_resolution_clock::now();
    uint32_t volk_conv_sse_index = 624;
    for(uint32_t k=0; k<num_gen; k++){
        volk_genrand_conv_sse(volk_conv_sse_value+k, volk_conv_sse_states, &volk_conv_sse_index);
    }
    t2 = high_resolution_clock::now();
    uint32_t runtime_volk_conv_sse = duration_cast<microseconds>(t2-t1).count();

    /* RUN BOOST RANDOM */
    double boost_value[num_gen];
    boost::mt19937 rng;
    boost::uniform_real<> uni_dist(0,1);
    boost::variate_generator<boost::mt19937, boost::uniform_real<double> > generator(rng, uni_dist);
    t1 = high_resolution_clock::now();
    for(uint32_t k=0; k<num_gen; k++){
        boost_value[k] = generator();
    }
    t2 = high_resolution_clock::now();
    std::cout << "Test BOOST: " << boost_value[0] << std::endl;
    uint32_t runtime_boost = duration_cast<microseconds>(t2-t1).count();

    /* RUN STANDARD MT WITH CONVERSION */
    double mt_std_value[num_gen];
    uint32_t mt_std_index = 624;
    t1 = high_resolution_clock::now();
    for(uint32_t k=0; k<num_gen; k++){
        mt_std_value[k] = mt_rndm_uint32(mt_std_states, &mt_std_index);
    }
    t2 = high_resolution_clock::now();
    std::cout << "Test MT Std: " << mt_std_value[0] << std::endl;
    uint32_t runtime_mt_std = duration_cast<microseconds>(t2-t1).count();

    /* RUN ORIGINAL CODE */
    double orig_value[num_gen];
    t1 = high_resolution_clock::now();
    for(uint32_t k=0; k<num_gen; k++){
        orig_value[k] = dsfmt_genrand_close1_open2(&dsfmt) - 1.0;
    }
    t2 = high_resolution_clock::now();
    uint32_t runtime_orig = duration_cast<microseconds>(t2-t1).count();

    /* COMPARE OUTPUTS */
    bool all_same = true;
    for(uint32_t k=0; k<num_gen; k++){
        if(not(volk_generic_value[k]==volk_sse_value[k])) all_same = false;
        if(not(volk_generic_value[k]==orig_value[k])) all_same = false;
    }
    if(all_same) std::cout << "Original code and VOLK (generic and SSE) implementations return the same values." << std::endl;
    else std::cout << "VOLK generic and SSE implementations DOES NOT return the same values." << std::endl;

    /* PRINT RUNTIMES */
    float runtime_comp = (float) runtime_mt_std;
    std::cout << "RUNTIME" << std::endl;
    std::cout << "    VOLK GENERIC: " << runtime_comp/runtime_volk_generic << std::endl;
    std::cout << "    VOLK GENERIC WITH CONVERSION: " << runtime_comp/runtime_volk_conv_generic << std::endl;
    std::cout << "    VOLK SSE2: " << runtime_comp/runtime_volk_sse << std::endl;
    std::cout << "    VOLK SSE2 WITH CONVERSION: " << runtime_comp/runtime_volk_conv_sse << std::endl;
    if(mode_original) std::cout << "    ORIG IMPL SSE2: " << runtime_comp/runtime_orig << std::endl;
    else  std::cout << "    ORIG IMPL GENERIC: " << runtime_comp/runtime_orig << std::endl;
    std::cout << "    BOOST: " << runtime_comp/runtime_boost << std::endl;
    std::cout << "    MT STD: " << runtime_comp/runtime_mt_std << std::endl;
}
