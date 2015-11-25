#include "../dsfmt_reference_impl/dSFMT.h"
#include <iostream>

/*
 * Mersenne Exponent. The period of the sequence
 *  is a multiple of 2^DSFMT_MEXP-1.
 */
#define DSFMT_MEXP 19937

/*
 * DSFMT generator has an internal state array of 128-bit integers,
 * and N is its size.
 */
#define DSFMT_N ((DSFMT_MEXP - 128) / 104 + 1)

/*
 * N32 is the size of internal state array when regarded as an array
 * of 32-bit integers.
 */
//#define DSFMT_N32 (DSFMT_N * 4) // FIXME

/*
 * N64 is the size of internal state array when regarded as an array
 * of 64-bit integers.
 */
#define DSFMT_N64 (DSFMT_N * 2)

/*
 * Algorithm parameters for Mersenne exponent of 19937.
 */
#define DSFMT_POS1	117
#define DSFMT_MSK32_1	0x000ffaffU
#define DSFMT_MSK32_2	0xfffffb3fU
#define DSFMT_MSK32_3	0x000ffdffU
#define DSFMT_MSK32_4	0xfc90fffdU
#define DSFMT_FIX1	UINT64_C(0x90014964b32f4329)
#define DSFMT_FIX2	UINT64_C(0x3b8d12ac548a7c7a)
#define DSFMT_PCV1	UINT64_C(0x3d84e1ac0dc82880)
#define DSFMT_PCV2	UINT64_C(0x0000000000000001)
#define DSFMT_SR	12
#define DSFMT_SSE2_SHUFF 0x1b
#define DSFMT_MSK1	UINT64_C(0x000ffafffffffb3f)
#define DSFMT_MSK2	UINT64_C(0x000ffdfffc90fffd)
#define DSFMT_SL1	19

inline static void dsfmt_recursion_generic(uint64_t *r, uint64_t *a, uint64_t *b, uint64_t *lung){
    uint64_t t0, t1, L0, L1;

    t0 = *a;
    t1 = *(a+1);
    L0 = *lung;
    L1 = *(lung+1);
    *lung = (t0 << DSFMT_SL1) ^ (L1 >> 32) ^ (L1 << 32) ^ *b;
    *(lung+1) = (t1 << DSFMT_SL1) ^ (L0 >> 32) ^ (L0 << 32) ^ *(b+1);
    *r = (*lung >> DSFMT_SR) ^ (*lung & DSFMT_MSK1) ^ t0;
    *(r+1) = (*(lung+1) >> DSFMT_SR) ^ (*(lung+1) & DSFMT_MSK2) ^ t1;
}

static inline void volk_sfmt_64f_genrand_generic(double *states){
    uint32_t i;
    uint64_t *pstate = (uint64_t*) states;
    uint64_t lung[2] = {*(pstate + 2*DSFMT_N), *(pstate + 2*DSFMT_N + 1)};

    dsfmt_recursion_generic(pstate, pstate, pstate + 2*DSFMT_POS1, lung);
    for (i = 1; i < DSFMT_N - DSFMT_POS1; i++){
        dsfmt_recursion_generic(pstate + 2*i, pstate + 2*i, pstate + 2*(i + DSFMT_POS1), lung);
    }
    for (; i < DSFMT_N; i++){
        dsfmt_recursion_generic(pstate + 2*i, pstate + 2*i, pstate + 2*(i + DSFMT_POS1 - DSFMT_N), lung);
    }
    *(pstate + 2*DSFMT_N) = lung[0];
    *(pstate + 2*DSFMT_N + 1) = lung[1];
}

#define DSFMT_LOW_MASK  UINT64_C(0x000FFFFFFFFFFFFF)
#define DSFMT_HIGH_CONST UINT64_C(0x3FF0000000000000)

static inline void volk_sfmt_64f_genrand_init_generic(double *states, uint32_t seed){
    uint32_t i;
    uint32_t *pstate = (uint32_t*) states;

    /* Run simple random number generator on states */
    pstate[0] = seed;
    for (i = 1; i < (DSFMT_N + 1) * 4; i++) {
        pstate[i] = 1812433253UL * (pstate[i - 1] ^ (pstate[i - 1] >> 30)) + i;
    }

    /* Init states to fit the IEEE 754 format */
    uint64_t *pstate64 = (uint64_t*) states;
    for (i = 0; i < DSFMT_N * 2; i++) {
        pstate64[i] = (pstate64[i] & DSFMT_LOW_MASK) | DSFMT_HIGH_CONST;
    }
}

int main(void){

}
