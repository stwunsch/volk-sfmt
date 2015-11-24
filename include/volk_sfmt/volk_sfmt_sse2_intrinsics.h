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
 * This file is intended to hold SSE2 intrinsics of intrinsics.
 * They should be used in VOLK kernels to avoid copy-pasta.
 */

#ifndef INCLUDE_VOLK_VOLK_SSE2_INTRINSICS_H_
#define INCLUDE_VOLK_VOLK_SSE2_INTRINSICS_H_

#include <emmintrin.h>

#define SFMT_MSK1	0xdfffffefU
#define SFMT_MSK2	0xddfecb7fU
#define SFMT_MSK3	0xbffaffffU
#define SFMT_MSK4	0xbffffff6U
#define SFMT_SR1	11
#define SFMT_SR2	1
#define SFMT_SL1	18
#define SFMT_SL2	1

static inline __m128i sfmt_recursion(__m128i a, __m128i b, __m128i c, __m128i d){
    __m128i v, x, y, z;

    static const uint32_t sfmt_param_mask[4] = {SFMT_MSK1, SFMT_MSK2, SFMT_MSK3, SFMT_MSK4};
    static const __m128i *_sfmt_param_mask = (__m128i*) sfmt_param_mask;

    y = _mm_srli_epi32(b, SFMT_SR1);
    z = _mm_srli_si128(c, SFMT_SR2);
    v = _mm_slli_epi32(d, SFMT_SL1);
    z = _mm_xor_si128(z, a);
    z = _mm_xor_si128(z, v);
    x = _mm_slli_si128(a, SFMT_SL2);
    y = _mm_and_si128(y, *_sfmt_param_mask);
    z = _mm_xor_si128(z, x);
    z = _mm_xor_si128(z, y);

    return z;
}

#define DSFMT_SR	12
#define DSFMT_SSE2_SHUFF 0x1b
#define DSFMT_MSK1	UINT64_C(0x000ffafffffffb3f)
#define DSFMT_MSK2	UINT64_C(0x000ffdfffc90fffd)
#define DSFMT_SL1	19

static inline void dsfmt_recursion(__m128i r, __m128i a, __m128i b, __m128i u){
    __m128i v, w, y, z;

    static const uint64_t dsfmt_param_mask[2] = {DSFMT_MSK1, DSFMT_MSK2};
    static const __m128i *_dsfmt_param_mask = (__m128i*) dsfmt_param_mask;

    z = _mm_slli_epi64(a, DSFMT_SL1);
    y = _mm_shuffle_epi32(u, DSFMT_SSE2_SHUFF);
    z = _mm_xor_si128(z, b);
    y = _mm_xor_si128(y, z);
    v = _mm_srli_epi64(y, DSFMT_SR);
    w = _mm_and_si128(y, *_dsfmt_param_mask);
    v = _mm_xor_si128(v, a);
    v = _mm_xor_si128(v, w);

    r = v;
    u = y;
}

#endif /* INCLUDE_VOLK_VOLK_SSE2_INTRINSICS_H_ */
