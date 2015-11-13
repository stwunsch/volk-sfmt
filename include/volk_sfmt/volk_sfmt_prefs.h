#ifndef INCLUDED_VOLK_SFMT_PREFS_H
#define INCLUDED_VOLK_SFMT_PREFS_H

#include <volk_sfmt/volk_sfmt_common.h>
#include <stdlib.h>

__VOLK_DECL_BEGIN

typedef struct volk_sfmt_arch_pref
{
    char name[128];   //name of the kernel
    char impl_a[128]; //best aligned impl
    char impl_u[128]; //best unaligned impl
} volk_sfmt_arch_pref_t;

////////////////////////////////////////////////////////////////////////
// get path to volk_sfmt_config profiling info;
// returns \0 in the argument on failure.
////////////////////////////////////////////////////////////////////////
VOLK_API void volk_sfmt_get_config_path(char *);

////////////////////////////////////////////////////////////////////////
// load prefs into global prefs struct
////////////////////////////////////////////////////////////////////////
VOLK_API size_t volk_sfmt_load_preferences(volk_sfmt_arch_pref_t **);

__VOLK_DECL_END

#endif //INCLUDED_VOLK_SFMT_PREFS_H
