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
struct array_elt
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
    struct array_elt *elts; // Array of struct array_elt
};

typedef struct json_array json_array_st;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
json_array_st *array_init(size_t size);

void array_append(json_array_st *ja, struct array_elt elt);

typed_value_st array_get(json_array_st *ja, size_t index);

#endif // !GENERIC_LISTS_H
