#ifndef RO_JSON_STORAGE_H
#define RO_JSON_STORAGE_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "base_json_storage.h"

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define ERROR_RO_VALUE ((ro_value_t){ .type = T_ERROR })
#define ERROR_RO_ITEM ((ro_item_t){ .type = T_ERROR })

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
typedef struct ro_array ro_array_t;
typedef struct ro_dict ro_dict_t;

// Used for functions that return an element of the array
typedef struct
{
    char type;
    union
    {
        string_t strv;
        int intv;
        double doublev;
        char boolv;
        ro_array_t *arrayv;
        ro_dict_t *dictv;
    };
} ro_value_t;

// Used for functions that return an element of the dict
typedef struct
{
    char type;
    string_t key;
    union
    {
        string_t strv;
        int intv;
        double doublev;
        char boolv;
        ro_array_t *arrayv;
        ro_dict_t *dictv;
    };
} ro_item_t;

struct ro_array
{
    unsigned size;
    unsigned insert_index;
    ro_value_t *values;
};

struct ro_dict
{
    unsigned size;
    unsigned insert_index;
    ro_item_t *items;
};

typedef struct
{
    char is_array;
    union
    {
        ro_array_t *array;
        ro_dict_t *dict;
    };
} ro_json_t;

/*******************************************************************************
**                                 FUNCTIONS **
*******************************************************************************/
ro_array_t *init_ro_array(unsigned size);
ro_dict_t *init_ro_dict(unsigned size);
ro_json_t *init_ro_json(char is_array, ro_array_t *a, ro_dict_t *d);

void ro_array_add_str(ro_array_t *a, string_t value);
void ro_array_add_int(ro_array_t *a, int value);
void ro_array_add_double(ro_array_t *a, double value);
void ro_array_add_bool(ro_array_t *a, char value);
void ro_array_add_null(ro_array_t *a);
void ro_array_add_array(ro_array_t *a, ro_array_t *value);
void ro_array_add_dict(ro_array_t *a, ro_dict_t *value);

void ro_dict_add_str(ro_dict_t *d, string_t key, string_t value);
void ro_dict_add_int(ro_dict_t *d, string_t key, int value);
void ro_dict_add_double(ro_dict_t *d, string_t key, double value);
void ro_dict_add_bool(ro_dict_t *d, string_t key, char value);
void ro_dict_add_null(ro_dict_t *d, string_t key);
void ro_dict_add_array(ro_dict_t *d, string_t key, ro_array_t *value);
void ro_dict_add_dict(ro_dict_t *d, string_t key, ro_dict_t *value);

ro_value_t ro_array_get(ro_array_t *a, unsigned index);
ro_item_t ro_dict_get(ro_dict_t *d, string_t key);

void ro_array_print(ro_array_t *a);
void ro_dict_print(ro_dict_t *d);

void destroy_ro_array(ro_array_t *a);
void destroy_ro_dict(ro_dict_t *d);
void destroy_ro_json(ro_json_t *j);

#endif // !RO_JSON_STORAGE_H
