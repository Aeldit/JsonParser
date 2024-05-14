#ifndef JSON_H
#define JSON_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stddef.h>

#include "lists/generic_lists.h"

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define TYPE_STR 0
#define TYPE_NUM 1
#define TYPE_OBJ 2
#define TYPE_ARR 3
#define TYPE_BOOL 4
#define TYPE_NULL 5

// Errors
#define TYPE_NONEXISTANT 6

#define ARRAY_LEN 8

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
typedef struct json_dict json_dict_st;

/**
** \brief Each '..._array_link' struct represents a link of the chained list of
**        the associated type. It contains an array of predetermined length and
**        a pointer to the previous link.
*/
struct pair_array_link
{
    struct pair *pairs[ARRAY_LEN];
    struct pair_array_link *next;
};

struct key_array_link
{
    const char *keys[ARRAY_LEN];
    struct key_array_link *next;
};

struct str_array_link
{
    char *strings[ARRAY_LEN];
    struct str_array_link *next;
};

struct num_array_link
{
    long numbers[ARRAY_LEN];
    struct num_array_link *next;
};

struct json_dict_array_link
{
    json_dict_st *json_dicts[ARRAY_LEN];
    struct json_dict_array_link *next;
};

struct list_array_link
{
    generic_list_st *lists[ARRAY_LEN];
    struct list_array_link *next;
};

struct bool_array_link
{
    char booleans[ARRAY_LEN];
    struct bool_array_link *next;
};

/**
** \brief Represents a linked list of the associated type (key, str, num, ...)
**        and stores the head of the list
*/
struct pair_control
{
    size_t nb_pairs;
    size_t idx;
    struct pair_array_link *head;
};

struct key_control
{
    size_t nb_keys;
    size_t idx;
    struct key_array_link *head;
};

struct str_control
{
    size_t nb_str;
    size_t idx;
    struct str_array_link *head;
};

struct num_control
{
    size_t nb_num;
    size_t idx;
    struct num_array_link *head;
};

struct json_dict_control
{
    size_t nb_json_dicts;
    size_t idx;
    struct json_dict_array_link *head;
};

struct list_control
{
    size_t nb_arr;
    size_t idx;
    struct list_array_link *head;
};

struct bool_control
{
    size_t nb_bool;
    size_t idx;
    struct bool_array_link *head;
};

typedef struct pair_control pair_control_st;
typedef struct key_control key_control_st;

typedef struct str_control str_control_st;
typedef struct num_control num_control_st;
typedef struct json_dict_control json_dict_control_st;
typedef struct list_control list_control_st;
typedef struct bool_control bool_control_st;

/**
** \brief Represents a key:value pair
** \param key The key
** \param value Points to the value of the type 'type' inside the corresponding
**        linked list
** \param type The type of the data to which 'value' points
*/
struct pair
{
    const char *key;
    void *value;
    unsigned char type;
};

/**
** \field strings Points to a linked list to be be able to have a new buffer
** \param strings Contains all the string values of the json object
** \param numbers Contains all the number values of the json object
** \param lists Contains all the list values of the json object
** \param booleans Contains all the boolean values of the json dict
*/
struct json_dict
{
    pair_control_st *pairs;
    key_control_st *keys;

    str_control_st *strings;
    num_control_st *numbers;
    json_dict_control_st *json_dicts;
    list_control_st *lists;
    bool_control_st *booleans;
};

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
/**
** \brief Initialises a json dict and returns it
*/
json_dict_st *init_dict(void);

/**
** \brief Adds a key:value pair of string type to the 'jd' json dict
*/
char add_str(json_dict_st *jd, const char *key, char *value);

/**
** \brief Adds a key:value pair of number type to the 'jd' json dict
*/
char add_num(json_dict_st *jd, const char *key, long value);

/**
** \brief Adds a key:value pair of json dict type to the 'jd' json dict
*/
char add_json_dict(json_dict_st *jd, const char *key, json_dict_st *dict);

/**
** \brief Adds a key:value pair of list type to the 'jd' json dict
*/
char add_list(json_dict_st *jd, const char *key, struct generic_list *list);

/**
** \brief Adds a key:value pair of boolean type to the 'jd' json dict
*/
char add_bool(json_dict_st *jd, const char *key, char value);

/**
** \brief Adds a key:value pair of null type to the 'jd' json dict
*/
char add_null(json_dict_st *jd, const char *key);

/**
** \brief Frees all the allocated parts of the 'jd' json dict
*/
void destroy_dict(json_dict_st *jd);

#endif // !JSON_H
