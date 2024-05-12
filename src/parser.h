#ifndef JSON_PARSER_H
#define JSON_PARSER_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stddef.h>

#include "lists/linked_lists.h"

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define TYPE_NULL 0
#define TYPE_STR 1
#define TYPE_NUM 2
#define TYPE_BOOL 3
#define TYPE_ARR 4
// #define TYPE_OBJ 2

#define BEGIN_ARRAY '['
#define END_ARRAY ']'
#define BEGIN_OBJECT '{'
#define END_OBJECT '}'
#define NAME_SEPARATOR ':'
#define VALUE_SEPARATOR ','

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
/**
** \brief
*/
struct pair
{
    const char *key; // Points to a key inside the 'keys' linked list
    void *value; // Points to a value inside the associated linked list
    unsigned char type;
};

/**
** \field strings Points to a linked list to be be able to have a new buffer
*/
struct json_dict
{
    pair_control_st *pairs;
    key_control_st *keys;

    str_control_st *strings;
    num_control_st *numbers;
    bool_control_st *booleans;
    list_control_st *lists;
};

typedef struct json_dict json_dict_st;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
json_dict_st *init_dict(void);

char add_null(json_dict_st *jd, const char *key);

char add_str(json_dict_st *jd, const char *key, char *value);

char add_num(json_dict_st *jd, const char *key, long value);

char add_bool(json_dict_st *jd, const char *key, char value);

char add_list(json_dict_st *jd, const char *key, struct generic_list *list);

void destroy_dict(json_dict_st *jd);

#endif // !JSON_PARSER_H
