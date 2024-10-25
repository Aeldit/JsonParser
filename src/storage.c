#include "storage.h"

#include <stdlib.h>

Array init_array(int size)
{
    return (Array){ .size = size, .insert_idx = 0 };
}

void arr_add_str(Array *a, String value)
{
    if (!a)
    {
        return;
    }

    if (!a->strings_head)
    {
        StrArrLink *l = calloc(1, sizeof(StrArrLink));
        if (!l)
        {
            return;
        }
        l->value = value;
        l->index = a->insert_idx;
        a->strings_head = l;
        a->strings_tail = l;
    }
    else
    {
        // Goes to the last element of the linked list and adds the new element
        StrArrLink *nl = calloc(1, sizeof(StrArrLink));
        if (!nl)
        {
            return;
        }

        nl->value = value;
        nl->index = a->insert_idx;
        a->strings_tail->next = nl;
        a->strings_tail = nl;
    }
    a->indexes_types[a->insert_idx++] = T_STR;
}

void arr_add_int(Array *a, int value)
{
    if (!a)
    {
        return;
    }

    if (!a->integers_head)
    {
        IntArrLink *l = calloc(1, sizeof(IntArrLink));
        if (!l)
        {
            return;
        }
        l->value = value;
        l->index = a->insert_idx;
        a->integers_head = l;
        a->integers_tail = l;
    }
    else
    {
        // Goes to the last element of the linked list and adds the new element
        IntArrLink *nl = calloc(1, sizeof(IntArrLink));
        if (!nl)
        {
            return;
        }
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
        if (!sl)
        {
            return (ArrayLink){ .type = T_ERROR };
        }
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
        if (!il)
        {
            return (ArrayLink){ .type = T_ERROR };
        }
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

    StrArrLink *sl = a->strings_head;
    while (sl)
    {
        StrArrLink *tmp = sl;
        sl = sl->next;
        free(tmp);
    }

    IntArrLink *il = a->integers_head;
    while (il)
    {
        IntArrLink *tmp = il;
        il = il->next;
        free(tmp);
    }
}
