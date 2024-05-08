#ifndef JSON_PARSER_H
#define JSON_PARSER_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stddef.h>

#include "linked_lists.h"

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define TYPE_STR 0
#define TYPE_NUM 1
#define TYPE_OBJ 2
#define TYPE_ARR 3
#define TYPE_BOOL 4

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
/**
** \brief
*/
struct pair
{
    size_t key_idx;
    size_t value_idx;
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
    // TODO: Objects + Arrays
    bool_control_st *booleans;
};

typedef struct json_dict json_dict_st;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
json_dict_st *init_dict(void);

char add_str(json_dict_st *jd, const char *key, const char *str);

void destroy_dict(json_dict_st *jd);

#endif // !JSON_PARSER_H
