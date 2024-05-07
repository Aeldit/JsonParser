#ifndef LINKED_LISTS_H
#define LINKED_LISTS_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stddef.h>

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define ARRAY_LEN 32

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
struct pair_array_link
{
    struct pair *pairs[ARRAY_LEN];
    struct pair_array_link *next;
};

struct key_array_link
{
    const char *keys[ARRAY_LEN];
    struct key_array_link *previous;
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

struct bool_array_link
{
    char booleans[ARRAY_LEN];
    struct bool_array_link *next;
};

struct key_control_t
{
    size_t nb_keys;
    size_t idx;
    struct key_array_link *head;
};

struct str_control_t
{
    size_t nb_str;
    size_t idx;
    struct str_array_link *head;
};

struct num_control_t
{
    size_t nb_num;
    size_t idx;
    struct num_array_link *head;
};

struct bool_control_t
{
    size_t nb_bool;
    size_t idx;
    struct bool_array_link *head;
};

typedef struct key_control_t key_control_t;
typedef struct str_control_t str_control_t;
typedef struct num_control_t num_control_t;
typedef struct bool_control_t bool_control_t;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/

#endif // !LINKED_LISTS_H
