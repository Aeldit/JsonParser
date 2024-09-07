#ifndef JSON_H
#define JSON_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdint.h>

#include "lists/json_array.h"

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define ARRAY_LEN 64

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
typedef struct json_dict json_dict_st;

/**
** \brief Each '..._link' struct represents a link of the chained list of
**        the associated type. It contains an array of predetermined length and
**        a pointer to the next link.
*/
struct item_link
{
    struct item *items[ARRAY_LEN];
    struct item_link *next;
};

struct key_link
{
    char *keys[ARRAY_LEN];
    struct key_link *next;
};

struct str_link
{
    char *strings[ARRAY_LEN];
    struct str_link *next;
};

struct int64_t_link
{
    int64_t integers[ARRAY_LEN];
    struct int64_t_link *next;
};

struct double_link
{
    double doubles[ARRAY_LEN];
    struct double_link *next;
};

struct bool_link
{
    char booleans[ARRAY_LEN];
    struct bool_link *next;
};

struct array_link
{
    json_array_st *lists[ARRAY_LEN];
    struct array_link *next;
};

struct json_dict_link
{
    json_dict_st *json_dicts[ARRAY_LEN];
    struct json_dict_link *next;
};

/**
** \brief Represents a linked list of the associated type (key, str, num, ...)
**        and stores the head of the list
** \param idx Stores the index of the next object that will be allocated
*/
struct item_control
{
    size_t idx;
    struct item_link *head;
};

struct key_control
{
    size_t idx;
    struct key_link *head;
};

struct str_control
{
    size_t nb_str;
    size_t idx;
    struct str_link *head;
};

struct int64_t_control
{
    size_t nb_int;
    size_t idx;
    struct int64_t_link *head;
};

struct double_control
{
    size_t nb_double;
    size_t idx;
    struct double_link *head;
};

struct bool_control
{
    size_t nb_bool;
    size_t idx;
    struct bool_link *head;
};

struct list_control
{
    size_t nb_arrays;
    size_t idx;
    struct array_link *head;
};

struct json_dict_control
{
    size_t nb_json_dicts;
    size_t idx;
    struct json_dict_link *head;
};

typedef struct item_control item_control_st;
typedef struct key_control key_control_st;

typedef struct str_control str_control_st;
typedef struct int64_t_control int64_t_control_st;
typedef struct double_control double_control_st;
typedef struct bool_control bool_control_st;
typedef struct list_control list_control_st;
typedef struct json_dict_control json_dict_control_st;

/**
** \brief Represents a key:value item
** \param key The key
** \param value Points to the value of the type 'type' inside the corresponding
**        linked list
** \param type The type of the data to which 'value' points
*/
struct item
{
    char *key;
    void *value;
    unsigned char type;
};

struct json_dict
{
    size_t nb_items;
    item_control_st *items;
    key_control_st *keys;

    str_control_st *strings;
    int64_t_control_st *integers;
    double_control_st *doubles;
    bool_control_st *booleans;
    list_control_st *lists;
    json_dict_control_st *json_dicts;
};

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
/**
** \brief Initialises a json dict and returns it
*/
json_dict_st *init_dict(void);

/***************************************
**             JSON OBJECT            **
***************************************/
/**
** \brief Adds a key:value pair of string type to the 'jd' json dict
*/
char add_str(json_dict_st *jd, char *key, char *value);

/**
** \brief Adds a key:value pair of number type to the 'jd' json dict
*/
char add_int(json_dict_st *jd, char *key, int64_t value);

/**
** \brief Adds a key:value pair of double type to the 'jd' json dict
*/
char add_double(json_dict_st *jd, char *key, double value);

/**
** \brief Adds a key:value pair of boolean type to the 'jd' json dict
*/
char add_bool(json_dict_st *jd, char *key, char value);

/**
** \brief Adds a key:value pair of null type to the 'jd' json dict
*/
char add_null(json_dict_st *jd, char *key);

/**
** \brief Adds a key:value pair of list type to the 'jd' json dict
*/
char add_array(json_dict_st *jd, char *key, json_array_st *list);

/**
** \brief Adds a key:value pair of json dict type to the 'jd' json dict
*/
char add_json_dict(json_dict_st *jd, char *key, json_dict_st *dict);

/***************************************
**                ARRAY               **
***************************************/
char add_str_to_array(json_dict_st *jd, json_array_st *ja, char *value);

char add_int_to_array(json_dict_st *jd, json_array_st *ja, int64_t value);

char add_double_to_array(json_dict_st *jd, json_array_st *ja, double value);

char add_bool_to_array(json_dict_st *jd, json_array_st *ja, char value);

char add_null_to_array(json_dict_st *jd, json_array_st *ja);

char add_array_to_array(json_dict_st *jd, json_array_st *ja,
                        json_array_st *value);

char add_json_dict_to_array(json_dict_st *jd, json_array_st *ja,
                            json_dict_st *value);

size_t get_nb_items(json_dict_st *jd);

#endif // !JSON_H
