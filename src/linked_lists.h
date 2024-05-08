#ifndef LINKED_LISTS_H
#define LINKED_LISTS_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stddef.h>

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define ARRAY_LEN 8

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
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
    const char *strings[ARRAY_LEN];
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
typedef struct bool_control bool_control_st;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
/***************************************
**               PAIR                 **
***************************************/
struct pair *append_pair(pair_control_st *pc, struct pair *p);

void print_pairs(pair_control_st *pc);

void destroy_pair_control(pair_control_st *p);

/***************************************
**                KEY                 **
***************************************/
/**
** \brief  Appends the given key to the keys array
** \return The index of the added key + 1 (0 is the error code), so we have to
**         make sure later to substract 1 to the result of this function
*/
const char *append_key(key_control_st *kl, const char *key);

void print_keys(key_control_st *kc);

void destroy_key_control(key_control_st *kc);

/***************************************
**                STR                 **
***************************************/
const char *append_str(str_control_st *sc, const char *str);

void destroy_str_control(str_control_st *sc);

/***************************************
**                NUM                 **
***************************************/
long *append_num(num_control_st *nc, long num);

void destroy_num_control(num_control_st *nc);

#endif // !LINKED_LISTS_H
