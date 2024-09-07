#ifndef VALUES_STORAGE_H
#define VALUES_STORAGE_H

#include <stdint.h>

#include "type_linked_lists.h"

struct values_storage
{
    ll_string_st strings;
    ll_int_st integers;
    ll_double_st doubles;
    ll_bool_st booleans; // TODO: Use bits shifts to gain some memory
    ll_array_st arrays;
    ll_dict_st dicts;
};

typedef struct values_storage storage_st;

char *store_string(storage_st *s, char *str);
int64_t *store_int(storage_st *s, int64_t i);
double *store_double(storage_st *s, double d);
char *store_boolean(storage_st *s, char b);
json_array_st *store_array(storage_st *s, json_array_st ja);
json_dict_st *store_dict(storage_st *s, json_dict_st jd);

void destroy_storage(storage_st *s);

#endif // !VALUES_STORAGE_H
