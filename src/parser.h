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
** \brief
** \field strings Points to a linked list to be be able to have a new buffer
*/
struct json_dict_t
{
    struct pair_array_link *pairs_head;

    key_control_t *keys;

    str_control_t *strings;
    num_control_t *numbers;
    // TODO: Objects + Arrays
    bool_control_t *booleans;
};

typedef struct json_dict_t json_dict_t;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
json_dict_t *init_dict(void);

char add_key(json_dict_t *jd, char *key);

void destroy_dict(json_dict_t *jd);

#endif // !JSON_PARSER_H
