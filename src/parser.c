#include "parser.h"

#include <stdlib.h>

#include "utils.h"

json_dict_t *init_dict(void)
{
    json_dict_t *jd = calloc(1, sizeof(json_dict_t *));
    return jd == NULL ? NULL : jd;
}

char add_key(json_dict_t *jd, char *key)
{
    if (jd == NULL || key == NULL)
    {
        return FAILURE;
    }

    if (jd->keys == NULL)
    {
        jd->keys = calloc(1, sizeof(key_control_t));
        if (jd->keys == NULL)
        {
            return FAILURE;
        }
    }
    append_key(jd->keys, key);
    return SUCCESS;
}

size_t get_nb_keys(json_dict_t *jd)
{
    return jd == NULL ? 0 : jd->keys == NULL ? 0 : jd->keys->nb_keys;
}

void destroy_dict(json_dict_t *jd)
{
    if (jd == NULL)
    {
        return;
    }

    destroy_key_control(jd->keys);
    // TODO: Destroy the other types
    free(jd);
}
