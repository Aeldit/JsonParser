#include "parser.h"

#include <stdlib.h>

#include "utils.h"

json_dict_st *init_dict(void)
{
    json_dict_st *jd = calloc(1, sizeof(json_dict_st *));
    return jd == NULL ? NULL : jd;
}

char add_key(json_dict_st *jd, const char *key)
{
    if (jd == NULL || key == NULL)
    {
        return FAILURE;
    }

    if (jd->keys == NULL)
    {
        jd->keys = calloc(1, sizeof(key_control_st));
        if (jd->keys == NULL)
        {
            return FAILURE;
        }
    }
    size_t idx = append_key(jd->keys, key);
    return SUCCESS;
}

size_t get_nb_keys(json_dict_st *jd)
{
    return jd == NULL ? 0 : jd->keys == NULL ? 0 : jd->keys->nb_keys;
}

void destroy_dict(json_dict_st *jd)
{
    if (jd == NULL)
    {
        return;
    }

    destroy_key_control(jd->keys);
    // TODO: Destroy the other types
    free(jd);
}
