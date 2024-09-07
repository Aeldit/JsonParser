#ifndef TYPE_LINKED_LIST_H
#define TYPE_LINKED_LIST_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdint.h>

#include "linked_list.h"

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
struct link_string
{
    char *string;
    struct link_string *next;
};

struct link_int
{
    int64_t i;
    struct link_int *next;
};

struct link_double
{
    double d;
    struct link_double *next;
};

struct link_bool
{
    char b;
    struct link_bool *next;
};

struct link_array
{
    json_array_st *ja;
    struct link_array *next;
};

struct link_dict
{
    json_dict_st *jd;
    struct link_dict *next;
};

struct ll_string
{
    uint64_t size;
    struct link_string *head;
    struct link_string *tail;
};

struct ll_int
{
    uint64_t size;
    struct link_int *head;
    struct link_int *tail;
};

struct ll_double
{
    uint64_t size;
    struct link_double *head;
    struct link_double *tail;
};

struct ll_bool
{
    uint64_t size;
    struct link_bool *head;
    struct link_bool *tail;
};

struct ll_array
{
    uint64_t size;
    struct link_array *head;
    struct link_array *tail;
};

struct ll_dict
{
    uint64_t size;
    struct link_dict *head;
    struct link_dict *tail;
};

typedef struct ll_string ll_string_st;
typedef struct ll_int ll_int_st;
typedef struct ll_double ll_double_st;
typedef struct ll_bool ll_bool_st;
typedef struct ll_array ll_array_st;
typedef struct ll_dict ll_dict_st;

#endif // TYPE_LINKED_LIST_H!
