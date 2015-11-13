/* -*- c++ -*- */
/*
 * Copyright 2015 Stefan Wunsch
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

#ifndef INCLUDED_volk_sfmt_32u_genrand_arraypuppet_32u_H
#define INCLUDED_volk_sfmt_32u_genrand_arraypuppet_32u_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <volk_sfmt/volk_sfmt_32u_genrand_array_32u.h>
#include <volk_sfmt/volk_sfmt.h>

#ifdef LV_HAVE_GENERIC

static inline void volk_sfmt_32u_genrand_arraypuppet_32u_generic(uint32_t *output, uint32_t *input, unsigned int num_points){
    if(num_points<624) printf("[volk_sfmt__32u_genrand_arraypuppet_32u_generic] Number of points is smaller than 624 (SFMT_N32).\n");
    else{
        uint32_t *states = (uint32_t*) volk_sfmt_malloc(4*SFMT_N32, volk_sfmt_get_alignment());
        memcpy(states,input,4*SFMT_N32);

        unsigned int num_points_mod = num_points - (num_points%4); // floor the number of points to multiple of 4, which are greater than SFMT_N32==624

        volk_sfmt_32u_genrand_array_32u_generic(output, states, num_points_mod);

        // set all elements that are not in valid range to zero (for assert in test case)
        for(unsigned int k = num_points_mod; k<num_points; k++) output[k] = 0;

        volk_sfmt_free(states);
    }
}

#endif // LV_HAVE_GENERIC

#ifdef LV_HAVE_SSE2

static inline void volk_sfmt_32u_genrand_arraypuppet_32u_a_sse2(uint32_t *output, uint32_t *input, unsigned int num_points){
    if(num_points<624) printf("[volk_sfmt_32u_genrand_arraypuppet_32u_a_sse2] Number of points is smaller than 624 (SFMT_N32).\n");
    else{
        uint32_t *states = (uint32_t*) volk_sfmt_malloc(4*SFMT_N32, volk_sfmt_get_alignment());
        memcpy(states,input,4*SFMT_N32);

        unsigned int num_points_mod = num_points - (num_points%4); // floor the number of points to multiple of 4, which are greater than SFMT_N32==624

        volk_sfmt_32u_genrand_array_32u_a_sse2(output, states, num_points_mod);

        // set all elements that are not in valid range to zero (for assert in test case)
        for(unsigned int k = num_points_mod; k<num_points; k++) output[k] = 0;

        volk_sfmt_free(states);
    }
}

#endif // LV_HAVE_SSE2

#endif // INCLUDED_volk_sfmt_32u_genrand_arraypuppet_32u_H
