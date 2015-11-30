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


#include <math.h>
#include <inttypes.h>

#ifndef INCLUDED_volk_sfmt_32f_boxmuller_32f_a_H
#define INCLUDED_volk_sfmt_32f_boxmuller_32f_a_H

#ifdef LV_HAVE_GENERIC
static inline void volk_sfmt_32f_boxmuller_32f_generic(float* output, float* input, unsigned int num_points){
    for(unsigned int k=0; k<num_points/2; k++){ // FIXME: only even input buffer size is allowed!
        const float calcSqrt = sqrtf(-2.0*logf(input[2*k]));
        output[2*k] = calcSqrt*cosf(2.0*M_PI*input[2*k+1]);
        output[2*k+1] = calcSqrt*sinf(2.0*M_PI*input[2*k+1]);
    }
}
#endif /* LV_HAVE_GENERIC */


#ifdef LV_HAVE_SSE2
static inline void volk_sfmt_32f_boxmuller_32f_a_sse2(float* output, float* input, unsigned int num_points){

}
#endif /* LV_HAVE_SSE2 */

#endif /* INCLUDED_volk_sfmt_32f_boxmuller_32f_a_H */
