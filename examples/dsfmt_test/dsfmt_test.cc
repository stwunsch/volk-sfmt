#include <volk_sfmt/volk_sfmt.h>
#include <volk_sfmt/volk_sfmt_64f_genrand.h>

#include <iostream>
#include "../dsfmt_reference_impl/dSFMT.h"

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

static inline void dsfmt_genrand(double* output, double* states, uint32_t* index){
    if(*index>=DSFMT_N64){
        if(0) volk_sfmt_64f_genrand_manual(states, "a_sse2");
        else volk_sfmt_64f_genrand_manual(states, "generic");
        *index = 0;
    }
    *output = states[*index];
    *index += 1;
}

int main (void){
    /* Setup */
    run_mode();

    /* Compare the init functions */
    size_t alignment = volk_sfmt_get_alignment();
    double *volk_states = (double*) volk_sfmt_malloc((DSFMT_N64+1)*2, alignment);
    uint32_t seed = 4357;
    volk_sfmt_64f_genrand_init(volk_states, seed);

    dsfmt_t dsfmt;
    double *dsfmt_states = (double*) dsfmt.status;
    dsfmt_chk_init_gen_rand(&dsfmt, seed, 19937);

    bool init_same = true;
    std::cout << "INIT" << std::endl;
    for(uint32_t k=0; k<DSFMT_N64; k++){
        if(not(volk_states[k]==dsfmt_states[k])) init_same = false;
        if(k<10) std::cout << k << " " << volk_states[k] << " " << dsfmt_states[k] << std::endl;
    }
    if(init_same) std::cout << "VOLK and original implementation perform the same init." << std::endl;
    else std::cout << "VOLK and original implementation DOES NOT perform the same init." << std::endl;

    /* Compare the generated pseudo-random numbers */
    uint32_t volk_index = DSFMT_N64;
    double volk_value, dsfmt_value;

    bool values_same = true;
    std::cout << "VALUES" << std::endl;
    for(uint32_t k=0; k<20; k++){
        dsfmt_genrand(&volk_value, volk_states, &volk_index);
        dsfmt_value = dsfmt_genrand_close1_open2(&dsfmt);
        if(not(volk_value==dsfmt_value)) values_same = false;
        std::cout << k << " " << volk_value << " " << dsfmt_value << std::endl;
    }
    if(values_same) std::cout << "VOLK and original implementation return the same values." << std::endl;
    else std::cout << "VOLK and original implementation DOES NOT return the same values." << std::endl;
}
