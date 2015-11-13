/* -*- c++ -*- */
/*
 * Copyright 2015 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
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

#ifndef INCLUDED_volk_sfmt_32u_genrand_array_32u_a_H
#define INCLUDED_volk_sfmt_32u_genrand_array_32u_a_H

/*
 * These functions are previously defined in volk_sfmt_32u_genrand.h
 */

inline static void sfmt_rshift128_generic(uint32_t *out, uint32_t const *in, const int shift);

inline static void sfmt_lshift128_generic(uint32_t *out, uint32_t const *in, const int shift);

inline static void sfmt_recursion_generic(uint32_t *r, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d);

#ifdef LV_HAVE_GENERIC

static inline void volk_sfmt_32u_genrand_array_32u_generic(uint32_t *output, uint32_t *states, unsigned int num_points){
    int i, j;
    uint32_t *r1;
    uint32_t *r2;
    uint32_t *pstate = (uint32_t*) states;
    uint32_t *parray = (uint32_t*) output;

    r1 = pstate + 4*(SFMT_N - 2);
    r2 = pstate + 4*(SFMT_N - 1);
    for (i = 0; i < SFMT_N - SFMT_POS1; i++){
        sfmt_recursion_generic(parray + 4*i, pstate + 4*i, pstate + 4*(i + SFMT_POS1), r1, r2);
        r1 = r2;
        r2 = parray + 4*i;
    }
    for (; i < SFMT_N; i++){
        sfmt_recursion_generic(parray + 4*i, pstate + 4*i, parray + 4*(i + SFMT_POS1 - SFMT_N), r1, r2);
        r1 = r2;
        r2 = parray + 4*i;
    }
    for (; i < num_points/4 - SFMT_N; i++){
        sfmt_recursion_generic(parray + 4*i, parray + 4*(i - SFMT_N), parray + 4*(i + SFMT_POS1 - SFMT_N), r1, r2);
        r1 = r2;
        r2 = parray + 4*i;
    }
    for(j = 0; j < (int)(2*SFMT_N - num_points/4); j++){
        *(pstate + 4*j) = *(parray + 4*(j + num_points/4 - SFMT_N));
    }
    for(; i < num_points/4; i++, j++){
        sfmt_recursion_generic(parray + 4*i, parray + 4*(i - SFMT_N), parray + 4*(i + SFMT_POS1 - SFMT_N), r1, r2);
        r1 = r2;
        r2 = parray + 4*i;
        *(pstate + 4*j) = *(parray + 4*i);
    }
}

#endif /* LV_HAVE_GENERIC */

#ifdef LV_HAVE_SSE2

#include <volk_sfmt/volk_sfmt_sse2_intrinsics.h>

static inline void volk_sfmt_32u_genrand_array_32u_a_sse2(uint32_t *output, uint32_t *states, unsigned int num_points){
    int i, j;
    __m128i r1, r2;
    __m128i *pstate = (__m128i*) states;
    __m128i *parray = (__m128i*) output;

    r1 = *(pstate + SFMT_N - 2);
    r2 = *(pstate + SFMT_N - 1);
    for (i = 0; i < SFMT_N - SFMT_POS1; i++){
        *(parray + i) = sfmt_recursion(*(pstate + i), *(pstate + i + SFMT_POS1), r1, r2);
        r1 = r2;
        r2 = *(parray + i);
    }
    for (; i < SFMT_N; i++){
        *(parray + i) = sfmt_recursion(*(pstate + i), *(parray + i + SFMT_POS1 - SFMT_N), r1, r2);
        r1 = r2;
        r2 = *(parray + i);
    }
    for (; i < num_points/4 - SFMT_N; i++){
        *(parray + i) = sfmt_recursion(*(parray + i - SFMT_N), *(parray + i + SFMT_POS1 - SFMT_N), r1, r2);
        r1 = r2;
        r2 = *(parray + i);
    }
    for (j = 0; j < (int)(2*SFMT_N - num_points/4); j++){
        *(pstate + j) = *(parray + j + num_points/4 - SFMT_N);
    }
    for(; i < num_points/4; i++, j++){
        *(parray + i) = sfmt_recursion(*(parray + i - SFMT_N), *(parray + i + SFMT_POS1 - SFMT_N), r1, r2);
        r1 = r2;
        r2 = *(parray + i);
        *(pstate + j) = *(parray + i);
    }
}

#endif /* LV_HAVE_SSE2 */

#endif // INCLUDED_volk_sfmt_32u_genrand_array_32u_a_H
