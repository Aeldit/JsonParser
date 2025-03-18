#ifndef RO_JSON_STORAGE_H
#define RO_JSON_STORAGE_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "base_json_storage.h"

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define ERROR_RO_VALUE ((ro_value_t){.type = T_ERROR})
#define ERROR_RO_ITEM ((ro_item_t){.type = T_ERROR})

#define RO_VALUE_OF(T_TYPE, type_field, value)                                 \
  ((ro_value_t){.type = (T_TYPE), .type_field = (value)})
#define RO_ITEM_OF(T_TYPE, type_field, value)                                  \
  ((ro_item_t){.type = (T_TYPE), .key = key, .type_field = (value)})

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
typedef struct ro_array ro_array_t;
typedef struct ro_dict ro_dict_t;

// Used for functions that return an element of the array
typedef struct {
  u8 type;
  union {
    string_t strv;
    i64 longv;
    double doublev;
    exp_long_t exp_longv;
    exp_double_t exp_doublev;
    bool boolv;
    ro_array_t *arrayv;
    ro_dict_t *dictv;
  };
} ro_value_t;

// Used for functions that return an element of the dict
typedef struct {
  u8 type;
  string_t key;
  union {
    string_t strv;
    i64 longv;
    double doublev;
    exp_long_t exp_longv;
    exp_double_t exp_doublev;
    bool boolv;
    ro_array_t *arrayv;
    ro_dict_t *dictv;
  };
} ro_item_t;

struct ro_array {
  size_t size;
  ro_value_t *values;
};

struct ro_dict {
  size_t size;
  ro_item_t *items;
};

typedef struct {
  bool is_array;
  union {
    ro_array_t *array;
    ro_dict_t *dict;
  };
} ro_json_t;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
ro_array_t *init_ro_array(size_t size);
ro_dict_t *init_ro_dict(size_t size);
ro_json_t *init_ro_json(bool is_array, ro_array_t *a, ro_dict_t *d);

ro_value_t ro_array_get(ro_array_t *a, size_t index);
ro_item_t ro_dict_get(ro_dict_t *d, string_t key);

void ro_array_print(ro_array_t *a);
void ro_dict_print(ro_dict_t *d);

void destroy_ro_array(ro_array_t *a);
void destroy_ro_dict(ro_dict_t *d);
void destroy_ro_json(ro_json_t *j);

#endif // !RO_JSON_STORAGE_H
