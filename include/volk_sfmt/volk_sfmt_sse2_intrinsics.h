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

static inline __m128i sfmt_recursion(__m128i a, __m128i b, __m128i c, __m128i d){
    __m128i v, x, y, z;

    static const uint32_t param_mask[4] = {SFMT_MSK1, SFMT_MSK2, SFMT_MSK3, SFMT_MSK4};
    static const __m128i *_param_mask = (__m128i*) param_mask;

    y = _mm_srli_epi32(b, SFMT_SR1);
    z = _mm_srli_si128(c, SFMT_SR2);
    v = _mm_slli_epi32(d, SFMT_SL1);
    z = _mm_xor_si128(z, a);
    z = _mm_xor_si128(z, v);
    x = _mm_slli_si128(a, SFMT_SL2);
    y = _mm_and_si128(y, *_param_mask);
    z = _mm_xor_si128(z, x);
    z = _mm_xor_si128(z, y);

    return z;
}

#endif /* INCLUDE_VOLK_VOLK_SSE2_INTRINSICS_H_ */
