#ifndef RW_JSON_STORAGE_H
#define RW_JSON_STORAGE_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdint.h>

#include "base_json_storage.h"

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define ERROR_RW_VALUE ((rw_value_t){ .type = T_ERROR })
#define ERROR_RW_ITEM ((rw_item_t){ .type = T_ERROR })

#define ARRAY_LEN 32
#define NB_DELETIONS_TO_DEFRAG 16

#ifdef LEAST
#    if ARRAY_LEN <= UINT_LEAST8_MAX
typedef u8 arr_size_t;
#    elif ARRAY_LEN <= UINT_LEAST16_MAX
typedef u16 arr_size_t;
#    elif ARRAY_LEN <= UINT_LEAST32_MAX
typedef u32 arr_size_t;
#    else
typedef u64 arr_size_t;
#    endif // !ARRAY_LEN
#else
#    if ARRAY_LEN <= UINT_FAST8_MAX
typedef u8 arr_size_t;
#    elif ARRAY_LEN <= UINT_FAST16_MAX
typedef u16 arr_size_t;
#    elif ARRAY_LEN <= UINT_FAST32_MAX
typedef u32 arr_size_t;
#    else
typedef u64 arr_size_t;
#    endif // !ARRAY_LEN
#endif // LEAST

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
typedef struct rw_array rw_array_t;
typedef struct rw_dict rw_dict_t;

// Used for functions that return an element of the array
typedef struct
{
    u8 type;
    union
    {
        string_t strv;
        i64 longv;
        double doublev;
        exponent_long_t exp_longv;
        exponent_double_t exp_doublev;
        bool boolv;
        rw_array_t *arrayv;
        rw_dict_t *dictv;
    };
} rw_value_t;

// Used for functions that return an element of the dict
typedef struct
{
    u8 type;
    string_t key;
    union
    {
        string_t strv;
        i64 longv;
        double doublev;
        exponent_long_t exp_longv;
        exponent_double_t exp_doublev;
        bool boolv;
        rw_array_t *arrayv;
        rw_dict_t *dictv;
    };
} rw_item_t;

typedef struct value_link
{
    rw_value_t values[ARRAY_LEN];
    arr_size_t insert_index;
    struct value_link *next;
} value_link_t;

typedef struct item_link
{
    rw_item_t items[ARRAY_LEN];
    arr_size_t insert_index;
    struct item_link *next;
} item_link_t;

struct rw_array
{
    size_t size;
    size_t nb_deletions;
    value_link_t *head;
    value_link_t *tail;
};

struct rw_dict
{
    size_t size;
    size_t nb_deletions;
    item_link_t *head;
    item_link_t *tail;
};

typedef struct
{
    bool is_array;
    union
    {
        rw_array_t *array;
        rw_dict_t *dict;
    };
} rw_json_t;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
rw_array_t *init_rw_array();
rw_dict_t *init_rw_dict();
rw_json_t *init_rw_json(bool is_array, rw_array_t *a, rw_dict_t *d);

void rw_array_add_str(rw_array_t *a, string_t value);
void rw_array_add_long(rw_array_t *a, i64 value);
void rw_array_add_double(rw_array_t *a, double value);
void rw_array_add_exp_long(rw_array_t *a, exponent_long_t value);
void rw_array_add_exp_double(rw_array_t *a, exponent_double_t value);
void rw_array_add_bool(rw_array_t *a, bool value);
void rw_array_add_null(rw_array_t *a);
// WARN: Check if the array is added to itself, and deep-copy it if so to
// prevent infinite recursion
void rw_array_add_array(rw_array_t *a, rw_array_t *value);
void rw_array_add_dict(rw_array_t *a, rw_dict_t *value);

void rw_dict_add_str(rw_dict_t *d, string_t key, string_t value);
void rw_dict_add_long(rw_dict_t *d, string_t key, i64 value);
void rw_dict_add_double(rw_dict_t *d, string_t key, double value);
void rw_dict_add_exp_long(rw_dict_t *d, string_t key, exponent_long_t value);
void rw_dict_add_exp_double(rw_dict_t *d, string_t key,
                            exponent_double_t value);
void rw_dict_add_bool(rw_dict_t *d, string_t key, bool value);
void rw_dict_add_null(rw_dict_t *d, string_t key);
void rw_dict_add_array(rw_dict_t *d, string_t key, rw_array_t *value);
// WARN: Check if the dict is added to itself, and deep-copy it if so to
// prevent infinite recursion
void rw_dict_add_dict(rw_dict_t *d, string_t key, rw_dict_t *value);

void rw_array_remove(rw_array_t *a, size_t index);
void rw_dict_remove(rw_dict_t *d, string_t key);

rw_value_t rw_array_get(rw_array_t *a, size_t index);
rw_item_t rw_dict_get(rw_dict_t *d, string_t key);

void rw_array_print(rw_array_t *a);
void rw_dict_print(rw_dict_t *d);

void destroy_rw_array(rw_array_t *a);
void destroy_rw_dict(rw_dict_t *d);
void destroy_rw_json(rw_json_t *j);

#endif // !RW_JSON_STORAGE_H
