#include "type_linked_lists.h"

#include <stdio.h>

void add_string(ll_string_st *l, char *string)
{
    if (l == NULL || string == NULL)
    {
        return;
    }
}

void add_int(ll_int_st *l, int64_t i);
void add_double(ll_double_st *l, double d);
void add_bool(ll_bool_st *l, char b);
void add_array(ll_array_st *l, json_array_st ja);
void add_dict(ll_dict_st *l, json_dict_st jd);
