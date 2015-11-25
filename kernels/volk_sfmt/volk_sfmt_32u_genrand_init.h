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

#ifndef INCLUDED_volk_sfmt_32u_genrand_init_a_H
#define INCLUDED_volk_sfmt_32u_genrand_init_a_H


#define DSFMT_MEXP 19937
#define SFMT_N (SFMT_MEXP / 128 + 1)
#define SFMT_N32 (SFMT_N * 4)

#ifdef LV_HAVE_GENERIC
static inline void volk_sfmt_32u_genrand_init_generic(uint32_t *states, uint32_t seed){
    uint32_t i;
    uint32_t *pstate = (uint32_t*) states;

    /* Run simple random number generator on states */
    pstate[0] = seed;
    for (i = 1; i < SFMT_N32; i++) {
        pstate[i] = 1812433253UL * (pstate[i - 1] ^ (pstate[i - 1] >> 30)) + i;
    }
}
#endif /* LV_HAVE_GENERIC */

#endif // INCLUDED_volk_sfmt_32u_genrand_init_a_H
