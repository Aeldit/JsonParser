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

struct str_array_link
{
    int index; // Index of the element in the array
    String value;
    struct str_array_link *next;
};

struct int_array_link
{
    int index; // Index of the element in the array
    int value;
    struct int_array_link *next;
};

struct double_array_link
{
    int index; // Index of the element in the array
    double value;
    struct double_array_link *next;
};

struct bool_array_link
{
    int index; // Index of the element in the array
    char value;
    struct bool_array_link *next;
};

struct null_array_link
{
    int index; // Index of the element in the array
    struct null_array_link *next;
};

struct arr_array_link
{
    int index; // Index of the element in the array
    Array *value;
    struct arr_array_link *next;
};

struct dict_array_link
{
    int index; // Index of the element in the array
    Dict *value;
    struct dict_array_link *next;
};

typedef struct str_array_link StrArrLink;
typedef struct int_array_link IntArrLink;
typedef struct double_array_link DoubleArrLink;
typedef struct bool_array_link BoolArrLink;
typedef struct null_array_link NullArrLink;
typedef struct arr_array_link ArrArrLink;
typedef struct dict_array_link DictArrLink;

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

// Linked list for arrays
struct array_linked_list
{
    int size;
    int insert_idx;

    // Stores for each index the type of the value, allowing us to not have to
    // search inside all linked lists
    char *indexes_types;

    // Pointers to the head of each linked list
    StrArrLink *strings_head;
    IntArrLink *integers_head;
    DoubleArrLink *doubles_head;
    BoolArrLink *booleans_head;
    NullArrLink *nulls_head;
    ArrArrLink *arrays_head;
    DictArrLink *dicts_head;

    // Stores the tail of each linked list, to allow faster access
    StrArrLink *strings_tail;
    IntArrLink *integers_tail;
    DoubleArrLink *doubles_tail;
    BoolArrLink *booleans_tail;
    NullArrLink *nulls_tail;
    ArrArrLink *arrays_tail;
    DictArrLink *dicts_tail;
};

struct str_dict_link
{
    String key;
    String value;
    struct str_dict_link *next;
};

struct int_dict_link
{
    String key;
    int value;
    struct int_dict_link *next;
};

struct double_dict_link
{
    String key;
    double value;
    struct double_dict_link *next;
};

struct bool_dict_link
{
    String key;
    char value;
    struct bool_dict_link *next;
};

struct null_dict_link
{
    String key;
    struct null_dict_link *next;
};

struct arr_dict_link
{
    String key;
    Array *value;
    struct arr_dict_link *next;
};

struct dict_dict_link
{
    String key;
    Dict *value;
    struct dict_dict_link *next;
};

struct generic_dict_link
{
    String key;
    Dict *value;
    struct dict_dict_link *next;
};

typedef struct str_dict_link StrDictLink;
typedef struct int_dict_link IntDictLink;
typedef struct double_dict_link DoubleDictLink;
typedef struct bool_dict_link BoolDictLink;
typedef struct null_dict_link NullDictLink;
typedef struct arr_dict_link ArrDictLink;
typedef struct dict_dict_link DictDictLink;

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

typedef struct
{
    String key;
    char type;
} KeyType;

// Linked list for dicts
struct dict_linked_list
{
    int size;
    int insert_idx;

    // Stores for each key the type of the associated value, allowing us to not
    // have to search inside all linked lists
    // The size of this array is set at the initialization of the parent struct
    KeyType *keys_types;

    // Pointers to the head of each linked list
    StrDictLink *strings_head;
    IntDictLink *integers_head;
    DoubleDictLink *doubles_head;
    BoolDictLink *booleans_head;
    NullDictLink *nulls_head;
    ArrDictLink *arrays_head;
    DictDictLink *dicts_head;

    // Stores the tail of each linked list, to allow faster access when adding
    // elements
    StrDictLink *strings_tail;
    IntDictLink *integers_tail;
    DoubleDictLink *doubles_tail;
    BoolDictLink *booleans_tail;
    NullDictLink *nulls_tail;
    ArrDictLink *arrays_tail;
    DictDictLink *dicts_tail;
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
/**
** \brief The size of the returned array is 0, it means that an error occured
**        when allocating the indexes_types array
*/
Array *init_array(int size);
Dict *init_dict(int size);
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

/**
** \returns A Value struct containing the type and the value of the correct type
*/
Value array_get(Array *a, int index);
Item dict_get(Dict *d, String key);

/**
** \brief Frees the allocated memory
*/
void destroy_array(Array *a);
void destroy_dict(Dict *d);
void destroy_json(JSON *j);

#endif // !STORAGE_H
