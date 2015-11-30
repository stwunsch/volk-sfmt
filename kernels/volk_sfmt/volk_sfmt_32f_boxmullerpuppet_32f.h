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


#include <volk_sfmt/volk_sfmt_32f_boxmuller_32f.h>
#include <volk_sfmt/volk_sfmt.h>
#include <stdio.h> // FIXME

#ifndef INCLUDED_volk_sfmt_32f_boxmullerpuppet_32f_H
#define INCLUDED_volk_sfmt_32f_boxmuller_32f_H

#ifdef LV_HAVE_GENERIC
static inline void volk_sfmt_32f_boxmullerpuppet_32f_generic(float* output, float* input, unsigned int num_points){
    float* input_trans = (float*) volk_sfmt_malloc(4*num_points, volk_sfmt_get_alignment());
    for(unsigned int k=0; k<num_points; k++) input_trans[k] = (input[k]+1.0)/2.0; // transform input from (-1,1) to (0,1)

    volk_sfmt_32f_boxmuller_32f_generic(output, input_trans, num_points);

    volk_sfmt_free(input_trans);
}
#endif /* LV_HAVE_GENERIC */


#ifdef LV_HAVE_SSE2
static inline void volk_sfmt_32f_boxmullerpuppet_32f_a_sse2(float* output, float* input, unsigned int num_points){
    float* input_trans = (float*) volk_sfmt_malloc(4*num_points, volk_sfmt_get_alignment());
    for(unsigned int k=0; k<num_points; k++) input_trans[k] = (input[k]+1.0)/2.0; // transform input from (-1,1) to (0,1)

    volk_sfmt_32f_boxmuller_32f_a_sse2(output, input_trans, num_points);

    volk_sfmt_free(input_trans);
}
#endif /* LV_HAVE_SSE2 */

#endif /* INCLUDED_volk_sfmt_32f_boxmullerpuppet_32f_H */
