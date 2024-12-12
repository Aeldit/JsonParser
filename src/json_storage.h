#ifndef JSON_STORAGE_H
#define JSON_STORAGE_H

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define T_ERROR 0
#define T_STR 1
#define T_INT 2
#define T_DOUBLE 3
#define T_BOOL 4
#define T_NULL 5
#define T_ARR 6
#define T_DICT 7

#define STRING_OF(string, len) ((string_t){ .str = (string), .length = (len) })
#define EMPTY_STRING ((string_t){ .str = 0, .length = 0 })

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
typedef struct
{
    char *str;
    unsigned length;
} string_t;

typedef struct array array_t;
typedef struct dict dict_t;

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
        array_t *arrayv;
        dict_t *dictv;
    };
} value_t;

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
        array_t *arrayv;
        dict_t *dictv;
    };
} item_t;

// FIX: Memory leak when EDITING_MODE is not defined
#define EDITING_MODE
#ifdef EDITING_MODE
#    define ARRAY_LEN 30
#    define NB_DELETIONS_TO_DEFRAG 16

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
#else
struct array
{
    unsigned size;
    unsigned insert_index;
    value_t *values;
};

struct dict
{
    unsigned size;
    unsigned insert_index;
    item_t *items;
};
#endif // !EDITING_MODE

typedef struct
{
    char is_array;
    union
    {
        array_t *array;
        dict_t *dict;
    };
} json_t;

/*******************************************************************************
**                                 FUNCTIONS **
*******************************************************************************/
char strings_equals(string_t s1, string_t s2);

json_t *init_json(char is_array, array_t *a, dict_t *d);

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

#ifdef EDITING_MODE
void arr_remove(array_t *a, unsigned index);
void dict_remove(dict_t *d, string_t key);
#else
array_t *init_array(unsigned size);
dict_t *init_dict(unsigned size);
#endif // !EDITING_MODE

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

#endif // !JSON_STORAGE_H
