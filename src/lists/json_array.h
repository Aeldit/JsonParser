#ifndef GENERIC_LISTS_H
#define GENERIC_LISTS_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stddef.h>

#include "../types.h"

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
struct list_elt
{
    void *value;
    unsigned char type;
};

/**
** \brief Represents a JSON array. It must be initialized with the correct
**        capacity before adding elements to it
*/
struct json_array
{
    size_t size;
    size_t idx;
    struct list_elt *elts;
};

typedef struct json_array json_array_st;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
void array_append(json_array_st *l, struct list_elt elt);

typed_value_st array_get(json_array_st *ja, size_t index);

void array_print(json_array_st *l);

void array_print_indent(json_array_st *l, char indent, char from_list);

#endif // !GENERIC_LISTS_H
