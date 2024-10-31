#ifndef STORAGE_H
#define STORAGE_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "json_types.h"

/*******************************************************************************
**                                   MACROS                                   **
*******************************************************************************/
/*
** \def Checks if the JSON instance is not null, if the JSON instance is an
**      array and if the array is not null
*/
#define IS_ARRAY(j) ((j) && (j->is_array) && (j->array))

/*
** \def Checks if the JSON instance is not null, if the JSON instance is a dict
**      and if the dict is not null
*/
#define IS_DICT(j) ((j) && !(j->is_array) && (j->dict))

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
typedef struct array_linked_list Array;
typedef struct dict_linked_list Dict;

// #define EDITING_MODE
#ifdef EDITING_MODE
typedef struct value_link
{
    char type;
    union
    {
        String strv;
        int intv;
        double doublev;
        char boolv;
        Array *arrayv;
        Dict *dictv;
    };
    struct value_link *next;
} ValueLink;

typedef struct item_link
{
    char type;
    String key;
    union
    {
        String strv;
        int intv;
        double doublev;
        char boolv;
        Array *arrayv;
        Dict *dictv;
    };
    struct item_link *next;
} ItemLink;
#endif // !EDITING_MODE

// Used for functions that return an element of the array
typedef struct
{
    char type;
    union
    {
        String strv;
        int intv;
        double doublev;
        char boolv;
        Array *arrayv;
        Dict *dictv;
    };
} Value;

// Used for functions that return an element of the dict
typedef struct
{
    char type;
    String key;
    union
    {
        String strv;
        int intv;
        double doublev;
        char boolv;
        Array *arrayv;
        Dict *dictv;
    };
} Item;

// Linked list for arrays
struct array_linked_list
{
    unsigned size;

#ifdef EDITING_MODE
    ValueLink *head;
    ValueLink *tail;
#else
    unsigned insert_index;
    Value *values;
#endif
};

// Linked list for dicts
struct dict_linked_list
{
    unsigned size;

#ifdef EDITING_MODE
    ItemLink *head;
    ItemLink *tail;
#else
    unsigned insert_index;
    Item *items;
#endif // !EDITING_MODE
};

typedef struct
{
    char is_array;
    union
    {
        Array *array;
        Dict *dict;
    };
} JSON;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
Array *init_array(unsigned size);
Dict *init_dict(unsigned size);
JSON *init_json(char is_array, Array *a, Dict *d);

void arr_add_str(Array *a, String value);
void arr_add_int(Array *a, int value);
void arr_add_double(Array *a, double value);
void arr_add_bool(Array *a, char value);
void arr_add_null(Array *a);
void arr_add_arr(Array *a, Array *value);
void arr_add_dict(Array *a, Dict *value);

void dict_add_str(Dict *d, String key, String value);
void dict_add_int(Dict *d, String key, int value);
void dict_add_double(Dict *d, String key, double value);
void dict_add_bool(Dict *d, String key, char value);
void dict_add_null(Dict *d, String key);
void dict_add_arr(Dict *d, String key, Array *value);
void dict_add_dict(Dict *d, String key, Dict *value);

#ifdef EDITING_MODE
void arr_remove(Array *a, unsigned index);
void dict_remove(Dict *d, String key);

void arr_insert_str(Array *a, unsigned index, String value);
void arr_insert_int(Array *a, unsigned index, int value);
void arr_insert_double(Array *a, unsigned index, double value);
void arr_insert_bool(Array *a, unsigned index, char value);
void arr_insert_null(Array *a, unsigned index);
void arr_insert_arr(Array *a, unsigned index, Array *value);
void arr_insert_dict(Array *a, unsigned index, Dict *value);
#endif // !EDITING_MODE

/**
** \returns A Value struct containing the type and the value of the correct type
*/
Value array_get(Array *a, unsigned index);
Item dict_get(Dict *d, String key);

/**
** \brief Frees the allocated memory
*/
void destroy_array(Array *a);
void destroy_dict(Dict *d);
void destroy_json(JSON *j);

#endif // !STORAGE_H
