/* -*- c++ -*- */
/*
 * Copyright 2015 Stefan Wunsch
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

/*
 * Copyright (C) 2006, 2007 Mutsuo Saito, Makoto Matsumoto and Hiroshima
 * University.
 * Copyright (C) 2012 Mutsuo Saito, Makoto Matsumoto, Hiroshima
 * University and The University of Tokyo.
 * Copyright (C) 2013 Mutsuo Saito, Makoto Matsumoto and Hiroshima
 * University.
 * All rights reserved.
 *
 * The 3-clause BSD License is applied to this software, see
 * LICENSE.txt
 */

/*
 * LICENSE.txt
 */

/*
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the names of Hiroshima University, The University of
      Tokyo nor the names of its contributors may be used to endorse
      or promote products derived from this software without specific
      prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
 * NOTES
 * - We assume LITTLE ENDIAN in this file.
 */

/*
 * Mersenne Exponent. The period of the sequence
 *  is a multiple of 2^MEXP-1.
 */
#define SFMT_MEXP 19937

/*
 * SFMT generator has an internal state array of 128-bit integers,
 * and N is its size.
 */
#define SFMT_N (SFMT_MEXP / 128 + 1)

/*
 * N32 is the size of internal state array when regarded as an array
 * of 32-bit integers.
 */
#define SFMT_N32 (SFMT_N * 4)

/*
 * the pick up position of the array
 */
#define SFMT_POS1	122

/*
 * the parameter of shift left as four 32-bit registers
 */
#define SFMT_SL1	18

/*
 * the parameter of shift left as one 128-bit register.
 * The 128-bit integer is shifted by (SFMT_SL2 * 8) bits.
 */
#define SFMT_SL2	1

/*
 * the parameter of shift right as four 32-bit registers
 */
#define SFMT_SR1	11

/*
 * the parameter of shift right as one 128-bit register.
 * The 128-bit integer is shifted by (SFMT_SL2 * 8) bits.
 */
#define SFMT_SR2	1

/*
 * A bitmask, used in the recursion.  These parameters are introduced
 * to break symmetry of SIMD.
 */
#define SFMT_MSK1	0xdfffffefU
#define SFMT_MSK2	0xddfecb7fU
#define SFMT_MSK3	0xbffaffffU
#define SFMT_MSK4	0xbffffff6U

/*
 * These definitions are part of a 128-bit period certification vector.
 */
#define SFMT_PARITY1	0x00000001U
#define SFMT_PARITY2	0x00000000U
#define SFMT_PARITY3	0x00000000U
#define SFMT_PARITY4	0x13c9e684U

#ifndef INCLUDED_volk_sfmt_32u_genrand_a_H
#define INCLUDED_volk_sfmt_32u_genrand_a_H

inline static void sfmt_rshift128_generic(uint32_t *out, uint32_t const *in, const int shift){
    static uint64_t th, tl, oh, ol;

    th = ((uint64_t)*(in+3) << 32) | ((uint64_t)*(in+2));
    tl = ((uint64_t)*(in+1) << 32) | ((uint64_t)*(in+0));

    oh = th >> (shift * 8);
    ol = tl >> (shift * 8);
    ol |= th << (64 - shift * 8);
    *(out+1) = (uint32_t)(ol >> 32);
    *(out+0) = (uint32_t)ol;
    *(out+3) = (uint32_t)(oh >> 32);
    *(out+2) = (uint32_t)oh;
}

inline static void sfmt_lshift128_generic(uint32_t *out, uint32_t const *in, const int shift){
    static uint64_t th, tl, oh, ol;

    th = ((uint64_t)*(in+3) << 32) | ((uint64_t)*(in+2));
    tl = ((uint64_t)*(in+1) << 32) | ((uint64_t)*(in+0));

    oh = th << (shift * 8);
    ol = tl << (shift * 8);
    oh |= tl >> (64 - shift * 8);
    *(out+1) = (uint32_t)(ol >> 32);
    *(out+0) = (uint32_t)ol;
    *(out+3) = (uint32_t)(oh >> 32);
    *(out+2) = (uint32_t)oh;
}

inline static void sfmt_recursion_generic(uint32_t *r, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d){
    static uint32_t x[4] = {0,0,0,0};
    static uint32_t y[4] = {0,0,0,0};

    sfmt_lshift128_generic(x, a, SFMT_SL2);
    sfmt_rshift128_generic(y, c, SFMT_SR2);
    *(r+0) = *(a+0) ^ *(x+0) ^ ((*(b+0) >> SFMT_SR1) & SFMT_MSK1) ^ *(y+0) ^ (*(d+0) << SFMT_SL1);
    *(r+1) = *(a+1) ^ *(x+1) ^ ((*(b+1) >> SFMT_SR1) & SFMT_MSK2) ^ *(y+1) ^ (*(d+1) << SFMT_SL1);
    *(r+2) = *(a+2) ^ *(x+2) ^ ((*(b+2) >> SFMT_SR1) & SFMT_MSK3) ^ *(y+2) ^ (*(d+2) << SFMT_SL1);
    *(r+3) = *(a+3) ^ *(x+3) ^ ((*(b+3) >> SFMT_SR1) & SFMT_MSK4) ^ *(y+3) ^ (*(d+3) << SFMT_SL1);
}

#ifdef LV_HAVE_GENERIC

static inline void volk_sfmt_32u_genrand_generic(uint32_t *states){
    int i;
    uint32_t *r1;
    uint32_t *r2;
    uint32_t *pstate = (uint32_t*) states;

    r1 = pstate + 4*(SFMT_N - 2);
    r2 = pstate + 4*(SFMT_N - 1);
    for (i = 0; i < SFMT_N - SFMT_POS1; i++){
        sfmt_recursion_generic(pstate + 4*i, pstate + 4*i, pstate + 4*(i + SFMT_POS1), r1, r2);
        r1 = r2;
        r2 = pstate + 4*i;
    }
    for (; i < SFMT_N; i++){
        sfmt_recursion_generic(pstate + 4*i, pstate + 4*i, pstate + 4*(i + SFMT_POS1 - SFMT_N), r1, r2);
        r1 = r2;
        r2 = pstate + 4*i;
    }
}

#endif /* LV_HAVE_GENERIC */

#ifdef LV_HAVE_SSE2

#include <volk_sfmt/volk_sfmt_sse2_intrinsics.h>

static inline void volk_sfmt_32u_genrand_a_sse2(uint32_t *states){
    int i;
    __m128i r1, r2;
    __m128i *pstate = (__m128i*) states;

    r1 = *(pstate + SFMT_N - 2);
    r2 = *(pstate + SFMT_N - 1);
    for (i = 0; i < SFMT_N - SFMT_POS1; i++){
        *(pstate + i) = sfmt_recursion(*(pstate + i), *(pstate + i + SFMT_POS1), r1, r2);
        r1 = r2;
        r2 = *(pstate + i);
    }
    for (; i < SFMT_N; i++){
        *(pstate + i) = sfmt_recursion(*(pstate + i), *(pstate + i + SFMT_POS1 - SFMT_N), r1, r2);
        r1 = r2;
        r2 = *(pstate + i);
    }
}

#endif /* LV_HAVE_SSE2 */

#endif // INCLUDED_volk_sfmt_32u_genrand_a_H
