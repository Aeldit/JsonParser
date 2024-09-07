#include "values_storage.h"

#include <stdio.h>
#include <stdlib.h>

#define STORE(ll_type_st, type_array, link_type, value_name, value)            \
    if (s == NULL)                                                             \
    {                                                                          \
        return NULL;                                                           \
    }                                                                          \
    ll_type_st *l = &s->type_array;                                            \
    if (l->size == 0)                                                          \
    {                                                                          \
        l->head = calloc(1, sizeof(struct link_type));                         \
        l->tail = l->head;                                                     \
    }                                                                          \
    l->tail->value_name = value

char *store_string(storage_st *s, char *str)
{
    if (str == NULL)
    {
        return NULL;
    }
    STORE(ll_string_st, strings, link_string, string, str);
    return l->tail->string;
}

int64_t *store_int(storage_st *s, int64_t i)
{
    STORE(ll_int_st, integers, link_int, i, i);
    return &l->tail->i;
}

double *store_double(storage_st *s, double d)
{
    STORE(ll_double_st, doubles, link_double, d, d);
    return &l->tail->d;
}

char *store_boolean(storage_st *s, char b)
{
    STORE(ll_bool_st, booleans, link_bool, b, b);
    return &l->tail->b;
}

json_array_st *store_array(storage_st *s, json_array_st ja)
{
    STORE(ll_array_st, arrays, link_array, ja, ja);
    return &l->tail->ja;
}

json_dict_st *store_dict(storage_st *s, json_dict_st jd)
{
    STORE(ll_dict_st, dicts, link_dict, jd, jd);
    return &l->tail->jd;
}
