#ifndef JSON_PARSER_H
#define JSON_PARSER_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stddef.h>

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define TYPE_STR 0
#define TYPE_NUM 1
#define TYPE_OBJ 2
#define TYPE_ARR 3
#define TYPE_BOOL 4

#define ARRAY_LEN 32

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
/**
** \brief
*/
typedef struct pair_t
{
    size_t key_idx;
    size_t value_idx;
    unsigned char type;
} pair_t;

struct key_link_t
{
    char *keys[ARRAY_LEN];
    struct key_link_t *next;
};

struct str_link_t
{
    char *strings[ARRAY_LEN];
    struct str_link_t *next;
};

struct num_link_t
{
    long numbers[ARRAY_LEN];
    struct num_link_t *next;
};

struct bool_link_t
{
    char booleans[ARRAY_LEN];
    struct bool_link_t *next;
};

typedef struct key_control_t
{
    size_t nb_keys;
    struct key_link_t *head;
} key_control_t;

typedef struct str_control_t
{
    size_t nb_str;
    struct str_link_t *head;
} str_control_t;

typedef struct num_control_t
{
    size_t nb_num;
    struct num_link_t *head;
} num_control_t;

typedef struct bool_control_t
{
    size_t nb_bool;
    struct bool_link_t *head;
} bool_control_t;

/**
** \brief
** \field strings Points to a linked list to be be able to have a new buffer
*/
typedef struct json_dict
{
    pair_t **pairs;

    key_control_t *keys;

    str_control_t *strings;
    num_link_t *numbers;
    // TODO: Objects + Arrays
    bool_control_t *booleans;
} json_dict;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
json_dict parse(char *file_path);

#endif // !JSON_PARSER_H
