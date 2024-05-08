#include "parser.h"

#include <stdlib.h>

#include "utils.h"

json_dict_st *init_dict(void)
{
    json_dict_st *jd = calloc(1, sizeof(json_dict_st *));
    return jd == NULL ? NULL : jd;
}

char add_str(json_dict_st *jd, const char *key, const char *str)
{
    if (jd == NULL || key == NULL || str == NULL)
    {
        return FAILURE;
    }

    if (jd->pairs == NULL)
    {
        jd->pairs = calloc(1, sizeof(pair_control_st));
    }
    if (jd->keys == NULL)
    {
        jd->keys = calloc(1, sizeof(key_control_st));
    }
    if (jd->strings == NULL)
    {
        jd->strings = calloc(1, sizeof(str_control_st));
    }

    if (jd->pairs == NULL || jd->keys == NULL || jd->strings == NULL)
    {
        destroy_pair_control(jd->pairs);
        destroy_key_control(jd->keys);
        destroy_str_control(jd->strings);
        return FAILURE;
    }

    struct pair *p = calloc(1, sizeof(struct pair));
    if (p == NULL)
    {
        return FAILURE;
    }
    p->type = TYPE_STR;
    p->key_idx = append_key(jd->keys, key);
    p->value_idx = append_str(jd->strings, str);
    append_pair(jd->pairs, p);
    return SUCCESS;
}

size_t get_nb_keys(json_dict_st *jd)
{
    return jd == NULL ? 0 : jd->keys == NULL ? 0 : jd->keys->nb_keys;
}

#include <stdio.h>
void destroy_dict(json_dict_st *jd)
{
    if (jd == NULL)
    {
        return;
    }

    destroy_pair_control(jd->pairs);
    destroy_key_control(jd->keys);
    destroy_str_control(jd->strings);
    // TODO: Destroy the other types
    free(jd);
}
