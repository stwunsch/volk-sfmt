/*
 * Copyright 2011-2012 Free Software Foundation, Inc.
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

#include <volk_sfmt/volk_sfmt_common.h>
#include <volk_sfmt/volk_sfmt_typedefs.h>
#include "volk_sfmt_machines.h"

struct volk_sfmt_machine *volk_sfmt_machines[] = {
#for $machine in $machines
#ifdef LV_MACHINE_$(machine.name.upper())
&volk_sfmt_machine_$(machine.name),
#endif
#end for
};

unsigned int n_volk_sfmt_machines = sizeof(volk_sfmt_machines)/sizeof(*volk_sfmt_machines);
