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

/* Copyright (C) 2007, 2008 Mutsuo Saito, Makoto Matsumoto and
 * Hiroshima University. All rights reserved.
 * Copyright (C) 2012 Mutsuo Saito, Makoto Matsumoto,
 * Hiroshima University and The University of Tokyo.
 * All rights reserved.
 *
 * The new BSD License is applied to this software.
 * see LICENSE.txt
 */

/*
 * LICENSE.txt
 */

/*
Copyright (c) 2007, 2008, 2009 Mutsuo Saito, Makoto Matsumoto
and Hiroshima University.
Copyright (c) 2011, 2002 Mutsuo Saito, Makoto Matsumoto, Hiroshima
University and The University of Tokyo.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of the Hiroshima University nor the names of
      its contributors may be used to endorse or promote products
      derived from this software without specific prior written
      permission.

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

#ifndef INCLUDED_volk_sfmt_64f_genrand_a_H
#define INCLUDED_volk_sfmt_64f_genrand_a_H

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

#ifdef LV_HAVE_GENERIC

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

#endif /* LV_HAVE_GENERIC */

#ifdef LV_HAVE_SSE2

#include <volk_sfmt/volk_sfmt_sse2_intrinsics.h>

static inline void volk_sfmt_64f_genrand_a_sse2(double *states){
    uint32_t i;
    __m128i *pstate = (__m128i*) states;
    __m128i lung = *(pstate + DSFMT_N);

    dsfmt_recursion(*pstate, *pstate, *(pstate + DSFMT_POS1), lung);
    for (i = 1; i < DSFMT_N - DSFMT_POS1; i++){
        dsfmt_recursion(*(pstate + i), *(pstate + i), *(pstate + i + DSFMT_POS1), lung);
    }
    for (; i < DSFMT_N; i++){
        dsfmt_recursion(*(pstate + i), *(pstate + i), *(pstate + i + DSFMT_POS1 - DSFMT_N), lung);
    }
    *(pstate + DSFMT_N) = lung;
}

#endif /* LV_HAVE_SSE2 */

#endif // INCLUDED_volk_sfmt_64f_genrand_a_H
