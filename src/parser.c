#include "parser.h"

#include <stdlib.h>

#include "utils.h"

json_dict_st *init_dict(void)
{
    json_dict_st *jd = calloc(1, sizeof(json_dict_st));
    return jd == NULL ? NULL : jd;
}

char add_str(json_dict_st *jd, const char *key, char *str)
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
        destroy_dict(jd);
        return FAILURE;
    }

    struct pair *p = calloc(1, sizeof(struct pair));
    if (p == NULL)
    {
        return FAILURE;
    }
    p->type = TYPE_STR;
    p->key = append_key(jd->keys, key);
    p->value = append_str(jd->strings, str);
    append_pair(jd->pairs, p);
    return SUCCESS;
}

char add_num(json_dict_st *jd, const char *key, long num)
{
    if (jd == NULL || key == NULL)
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
    if (jd->numbers == NULL)
    {
        jd->numbers = calloc(1, sizeof(num_control_st));
    }

    if (jd->pairs == NULL || jd->keys == NULL || jd->numbers == NULL)
    {
        destroy_dict(jd);
        return FAILURE;
    }

    struct pair *p = calloc(1, sizeof(struct pair));
    if (p == NULL)
    {
        return FAILURE;
    }
    p->type = TYPE_NUM;
    p->key = append_key(jd->keys, key);
    p->value = append_num(jd->numbers, num);
    append_pair(jd->pairs, p);
    return SUCCESS;
}

char add_bool(json_dict_st *jd, const char *key, char boolean)
{
    if (jd == NULL || key == NULL)
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
    if (jd->booleans == NULL)
    {
        jd->booleans = calloc(1, sizeof(bool_control_st));
    }

    if (jd->pairs == NULL || jd->keys == NULL || jd->booleans == NULL)
    {
        destroy_dict(jd);
        return FAILURE;
    }

    struct pair *p = calloc(1, sizeof(struct pair));
    if (p == NULL)
    {
        return FAILURE;
    }
    p->type = TYPE_BOOL;
    p->key = append_key(jd->keys, key);
    p->value = append_bool(jd->booleans, boolean);
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
    destroy_num_control(jd->numbers);
    destroy_bool_control(jd->booleans);
    //  TODO: Destroy the other types
    free(jd);
}
