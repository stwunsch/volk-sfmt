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
 * sfmt generator has an internal state array of 128-bit integers,
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
//#define DSFMT_N64 (DSFMT_N * 2) // FIXME

/*
 * Algorithm parameters for Mersenne exponent of 19937.
 */
#define DSFMT_POS1	117
#define DSFMT_SL1	19
#define DSFMT_MSK1	UINT64_C(0x000ffafffffffb3f)
#define DSFMT_MSK2	UINT64_C(0x000ffdfffc90fffd)
#define DSFMT_MSK32_1	0x000ffaffU
#define DSFMT_MSK32_2	0xfffffb3fU
#define DSFMT_MSK32_3	0x000ffdffU
#define DSFMT_MSK32_4	0xfc90fffdU
#define DSFMT_FIX1	UINT64_C(0x90014964b32f4329)
#define DSFMT_FIX2	UINT64_C(0x3b8d12ac548a7c7a)
#define DSFMT_PCV1	UINT64_C(0x3d84e1ac0dc82880)
#define DSFMT_PCV2	UINT64_C(0x0000000000000001)
#define DSFMT_SSE2_SHUFF 0x1b
#define DSFMT_SR	12

#ifndef INCLUDED_volk_sfmt_64f_genrand_a_H
#define INCLUDED_volk_sfmt_64f_genrand_a_H

#ifdef LV_HAVE_GENERIC

static inline void volk_sfmt_64f_genrand_generic(double *states){
}

#endif /* LV_HAVE_GENERIC */

#ifdef LV_HAVE_SSE2

#include <volk_sfmt/volk_sfmt_sse2_intrinsics.h>

static inline void volk_sfmt_64f_genrand_a_sse2(double *states){
    //int i;
    __m128i *pstate = (__m128i*) states;

    dsfmt_recursion(*(pstate), *(pstate), *(pstate + DSFMT_POS1), *(pstate + DSFMT_N));
}

#endif /* LV_HAVE_SSE2 */

#endif // INCLUDED_volk_sfmt_64f_genrand_a_H
