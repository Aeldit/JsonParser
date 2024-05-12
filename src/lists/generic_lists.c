#include "generic_lists.h"

#include <stdlib.h>

void append(generic_list_st *l, struct list_elt elt)
{
    if (l == NULL || l->idx >= l->size - 1)
    {
        return;
    }
    if (l->elts == NULL)
    {
        l->elts = calloc(l->size, sizeof(struct list_elt));
        if (l->elts == NULL)
        {
            return;
        }
    }
    l->elts[l->idx++] = elt;
}
