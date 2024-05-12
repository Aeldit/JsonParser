#ifndef JSON_H
#define JSON_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stddef.h>

#include "lists/linked_lists.h"

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define TYPE_STR 0
#define TYPE_NUM 1
#define TYPE_OBJ 2
#define TYPE_ARR 3
#define TYPE_BOOL 4
#define TYPE_NULL 5

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
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
    list_control_st *lists;
    bool_control_st *booleans;
};

typedef struct json_dict json_dict_st;

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
