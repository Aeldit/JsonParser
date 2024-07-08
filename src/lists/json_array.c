#include "json_array.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
json_array_st *array_init(size_t size)
{
    if (size == 0)
    {
        return NULL;
    }

    json_array_st *ja = calloc(1, sizeof(json_array_st));
    if (ja == NULL)
    {
        return NULL;
    }
    ja->size = size;
    ja->elts = calloc(size, sizeof(struct array_elt));
    if (ja->elts == NULL)
    {
        free(ja);
        return NULL;
    }
    return ja;
}

void array_append(json_array_st *ja, struct array_elt elt)
{
    if (ja == NULL || ja->idx >= ja->size
        || (elt.value == NULL && elt.type != TYPE_NULL))
    {
        return;
    }
    ja->elts[ja->idx++] = elt;
}

typed_value_st array_get(json_array_st *ja, size_t index)
{
    if (ja == NULL || index >= ja->size)
    {
        return (typed_value_st){ .type = TYPE_ERROR, .value = NULL };
    }
    return (typed_value_st){ .type = ja->elts[index].type,
                             .value = ja->elts[index].value };
}
