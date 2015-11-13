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

#ifndef INCLUDED_volk_32u_sfmt_genrandpuppet_32u_H
#define INCLUDED_volk_32u_sfmt_genrandpuppet_32u_H

#include <stdint.h>
#include <stdio.h>
#include <volk_sfmt/volk_sfmt_32u_genrand.h>

#ifdef LV_HAVE_GENERIC

static inline void volk_sfmt_32u_genrandpuppet_32u_generic(uint32_t *output, uint32_t *input, unsigned int num_points){
    if(num_points<624) printf("[volk_sfmt_32u_genrandpuppet_32u_generic] Number of points is smaller than 624 (SFMT_N32).\n");
    else{
        for(unsigned int k=0; k<624; k++) output[k] = input[k];

        volk_sfmt_32u_genrand_generic(output);

        // set all elements except first one to zero (for assert in test case)
        for(unsigned int k = 624; k<num_points; k++) output[k] = 0;
    }
}

#endif // LV_HAVE_GENERIC

#ifdef LV_HAVE_SSE2

static inline void volk_sfmt_32u_genrandpuppet_32u_a_sse2(uint32_t *output, uint32_t *input, unsigned int num_points){
    if(num_points<624) printf("[volk_sfmt_32u_genrandpuppet_32u_a_sse2] Number of points is smaller than 624 (SFMT_N32).\n");
    else{
        for(unsigned int k=0; k<624; k++) output[k] = input[k];

        volk_sfmt_32u_genrand_a_sse2(output);

        // set all elements except first one to zero (for assert in test case)
        for(unsigned int k = 624; k<num_points; k++) output[k] = 0;
    }
}

#endif // LV_HAVE_SSE2

#endif // INCLUDED_volk_sfmt_32u_genrandpuppet_32u_H
