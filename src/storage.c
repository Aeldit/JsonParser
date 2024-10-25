#include "storage.h"

#include <stdlib.h>

Array init_array(int size)
{
    return (Array){ .size = size };
}

void arr_add_int(Array *a, int value)
{
    if (!a)
    {
        return;
    }

    if (a->insert_idx == 0)
    {
        IntArrLink *l = calloc(1, sizeof(IntArrLink));
        l->value = value;
        a->integers_head = l;
        a->integers_tail = l;
    }
    else
    {
        // Goes to the last element of the linked list and adds the new element
        IntArrLink *nl = calloc(1, sizeof(IntArrLink));
        nl->value = value;
        nl->index = a->insert_idx;
        a->integers_tail->next = nl;
        a->integers_tail = nl;
    }
    a->indexes_types[a->insert_idx++] = T_INT;
}

ArrayLink array_get(Array *a, int index)
{
    if (!a)
    {
        return (ArrayLink){ .type = T_ERROR };
    }

    StrArrLink *sl = 0;
    IntArrLink *il = 0;
    switch (a->indexes_types[index])
    {
    case T_STR:
        sl = a->strings_head;
        while (sl->next)
        {
            if (sl->index == index)
            {
                break;
            }
            sl = sl->next;
        }
        return (ArrayLink){ .type = T_STR, .str = sl->value };
    case T_INT:
        il = a->integers_head;
        while (il->next)
        {
            if (il->index == index)
            {
                break;
            }
            il = il->next;
        }
        return (ArrayLink){ .type = T_INT, .integer = il->value };
    }
    return (ArrayLink){ .type = T_ERROR };
}

void destroy_array(Array *a)
{
    // free(a->indexes_types);

    IntArrLink *il = a->integers_head;
    while (il)
    {
        IntArrLink *tmp = il;
        il = il->next;
        free(tmp);
    }
}
