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


#ifndef INCLUDED_volk_sfmt_64f_genrand_init_a_H
#define INCLUDED_volk_sfmt_64f_genrand_init_a_H


#define DSFMT_MEXP 19937
#define DSFMT_N ((DSFMT_MEXP - 128) / 104 + 1)

#define DSFMT_LOW_MASK  UINT64_C(0x000FFFFFFFFFFFFF)
#define DSFMT_HIGH_CONST UINT64_C(0x3FF0000000000000)

#ifdef LV_HAVE_GENERIC
static inline void volk_sfmt_64f_genrand_init_generic(double *states, uint32_t seed){
    uint32_t i;
    uint32_t *pstate = (uint32_t*) states;

    /* Run simple random number generator on states */
    pstate[0] = seed;
    for (i = 1; i < (DSFMT_N + 1) * 4; i++) {
        pstate[i] = 1812433253UL * (pstate[i - 1] ^ (pstate[i - 1] >> 30)) + i;
    }

    /* Init states to fit the IEEE 754 format */
    uint64_t *pstate64 = (uint64_t*) states;
    for (i = 0; i < DSFMT_N * 2; i++) {
        pstate64[i] = (pstate64[i] & DSFMT_LOW_MASK) | DSFMT_HIGH_CONST;
    }
}
#endif /* LV_HAVE_GENERIC */

#endif // INCLUDED_volk_sfmt_64f_genrand_init_a_H
