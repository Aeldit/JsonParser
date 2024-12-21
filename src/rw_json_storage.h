#ifndef RW_JSON_STORAGE_H
#define RW_JSON_STORAGE_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "base_json_storage.h"

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define ARRAY_LEN 30
#define NB_DELETIONS_TO_DEFRAG 16

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
typedef struct value_link
{
    value_t values[ARRAY_LEN];
    unsigned insert_index;
    struct value_link *next;
} value_link_t;

typedef struct item_link
{
    item_t items[ARRAY_LEN];
    unsigned insert_index;
    struct item_link *next;
} item_link_t;

struct array
{
    unsigned size;
    unsigned nb_deletions;
    value_link_t *head;
    value_link_t *tail;
};

struct dict
{
    unsigned size;
    unsigned nb_deletions;
    item_link_t *head;
    item_link_t *tail;
};

/*******************************************************************************
**                                 FUNCTIONS **
*******************************************************************************/
void arr_add_str(array_t *a, string_t value);
void arr_add_int(array_t *a, int value);
void arr_add_double(array_t *a, double value);
void arr_add_bool(array_t *a, char value);
void arr_add_null(array_t *a);
// WARN: Check if the array is added to itself, and deep-copy it if so to
// prevent infinite recursion
void arr_add_arr(array_t *a, array_t *value);
void arr_add_dict(array_t *a, dict_t *value);

void dict_add_str(dict_t *d, string_t key, string_t value);
void dict_add_int(dict_t *d, string_t key, int value);
void dict_add_double(dict_t *d, string_t key, double value);
void dict_add_bool(dict_t *d, string_t key, char value);
void dict_add_null(dict_t *d, string_t key);
void dict_add_arr(dict_t *d, string_t key, array_t *value);
// WARN: Check if the dict is added to itself, and deep-copy it if so to
// prevent infinite recursion
void dict_add_dict(dict_t *d, string_t key, dict_t *value);

void arr_remove(array_t *a, unsigned index);
void dict_remove(dict_t *d, string_t key);

/**
** \returns A Value struct containing the type and the value of the correct
*type
*/
value_t array_get(array_t *a, unsigned index);
item_t dict_get(dict_t *d, string_t key);

/**
** \brief Frees the allocated memory
*/
void destroy_array(array_t *a);
void destroy_dict(dict_t *d);
void destroy_json(json_t *j);

#endif // !RW_JSON_STORAGE_H
