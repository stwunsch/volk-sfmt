#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <volk_sfmt/volk_sfmt_prefs.h>

void volk_sfmt_get_config_path(char *path)
{
    if (!path) return;
    const char *suffix = "/.volk_sfmt/volk_sfmt_config";
    char *home = NULL;
    if (home == NULL) home = getenv("HOME");
    if (home == NULL) home = getenv("APPDATA");
    if (home == NULL){
        path[0] = 0;
        return;
    }
    strncpy(path, home, 512);
    strcat(path, suffix);
}

size_t volk_sfmt_load_preferences(volk_sfmt_arch_pref_t **prefs_res)
{
    FILE *config_file;
    char path[512], line[512];
    size_t n_arch_prefs = 0;
    volk_sfmt_arch_pref_t *prefs = NULL;

    //get the config path
    volk_sfmt_get_config_path(path);
    if (!path[0]) return n_arch_prefs; //no prefs found
    config_file = fopen(path, "r");
    if(!config_file) return n_arch_prefs; //no prefs found

    //reset the file pointer and write the prefs into volk_sfmt_arch_prefs
    while(fgets(line, sizeof(line), config_file) != NULL)
    {
        prefs = (volk_sfmt_arch_pref_t *) realloc(prefs, (n_arch_prefs+1) * sizeof(*prefs));
        volk_sfmt_arch_pref_t *p = prefs + n_arch_prefs;
        if(sscanf(line, "%s %s %s", p->name, p->impl_a, p->impl_u) == 3 && !strncmp(p->name, "volk_sfmt_", 5))
        {
            n_arch_prefs++;
        }
    }
    fclose(config_file);
    *prefs_res = prefs;
    return n_arch_prefs;
}
