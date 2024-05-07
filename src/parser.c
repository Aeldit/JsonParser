#include "parser.h"

#include <stdlib.h>

#include "utils.h"

json_dict_t *init_dict()
{
    json_dict_t *jd = calloc(1, sizeof(json_dict_t *));
    if (jd == NULL)
    {
        return NULL;
    }

    struct pair_array_link *pl = calloc(1, sizeof(struct pair_array_link *));
    key_control_t *kl = calloc(1, sizeof(key_control_t *));
    str_control_t *sl = calloc(1, sizeof(str_control_t *));
    num_control_t *nl = calloc(1, sizeof(num_control_t *));
    bool_control_t *bl = calloc(1, sizeof(bool_control_t *));
    if (pl == NULL || kl == NULL || sl == NULL || nl == NULL || bl == NULL)
    {
        free(pl);
        free(kl);
        free(sl);
        free(nl);
        free(bl);
        return NULL;
    }
    return jd;
}

char add_key(json_dict_t *jd, char *key)
{
    if (jd == NULL || key == NULL)
    {
        return FAILURE;
    }

    jd->keys;
    return SUCCESS;
}

char add_str(json_dict_t *jd, char *key, char *str)
{
    if (jd == NULL || key == NULL || str == NULL)
    {
        return FAILURE;
    }
    return SUCCESS;
}

size_t get_nb_keys(json_dict_t *jd)
{
    return jd == NULL ? 0 : jd->keys == NULL ? 0 : jd->keys->nb_keys;
}
