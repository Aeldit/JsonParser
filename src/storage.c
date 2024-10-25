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

void arr_add_double(Array *a, double value)
{
    if (!a)
    {
        return;
    }

    if (!a->doubles_head)
    {
        DoubleArrLink *l = calloc(1, sizeof(DoubleArrLink));
        if (!l)
        {
            return;
        }
        l->value = value;
        l->index = a->insert_idx;
        a->doubles_head = l;
        a->doubles_tail = l;
    }
    else
    {
        // Goes to the last element of the linked list and adds the new element
        DoubleArrLink *nl = calloc(1, sizeof(DoubleArrLink));
        if (!nl)
        {
            return;
        }
        nl->value = value;
        nl->index = a->insert_idx;
        a->doubles_tail->next = nl;
        a->doubles_tail = nl;
    }
    a->indexes_types[a->insert_idx++] = T_DOUBLE;
}

void arr_add_bool(Array *a, char value)
{
    if (!a)
    {
        return;
    }

    if (!a->booleans_head)
    {
        BoolArrLink *l = calloc(1, sizeof(BoolArrLink));
        if (!l)
        {
            return;
        }
        l->value = value;
        l->index = a->insert_idx;
        a->booleans_head = l;
        a->booleans_tail = l;
    }
    else
    {
        // Goes to the last element of the linked list and adds the new element
        BoolArrLink *nl = calloc(1, sizeof(BoolArrLink));
        if (!nl)
        {
            return;
        }
        nl->value = value;
        nl->index = a->insert_idx;
        a->booleans_tail->next = nl;
        a->booleans_tail = nl;
    }
    a->indexes_types[a->insert_idx++] = T_BOOL;
}

void arr_add_null(Array *a)
{
    if (!a)
    {
        return;
    }

    if (!a->nulls_head)
    {
        NullArrLink *l = calloc(1, sizeof(NullArrLink));
        if (!l)
        {
            return;
        }
        l->index = a->insert_idx;
        a->nulls_head = l;
        a->nulls_tail = l;
    }
    else
    {
        // Goes to the last element of the linked list and adds the new element
        NullArrLink *nl = calloc(1, sizeof(NullArrLink));
        if (!nl)
        {
            return;
        }
        nl->index = a->insert_idx;
        a->nulls_tail->next = nl;
        a->nulls_tail = nl;
    }
    a->indexes_types[a->insert_idx++] = T_NULL;
}

void arr_add_array(Array *a, Array *value)
{
    if (!a || !value)
    {
        return;
    }

    if (!a->arrays_head)
    {
        ArrArrLink *l = calloc(1, sizeof(ArrArrLink));
        if (!l)
        {
            return;
        }
        l->value = value;
        l->index = a->insert_idx;
        a->arrays_head = l;
        a->arrays_tail = l;
    }
    else
    {
        // Goes to the last element of the linked list and adds the new element
        ArrArrLink *nl = calloc(1, sizeof(ArrArrLink));
        if (!nl)
        {
            return;
        }
        nl->value = value;
        nl->index = a->insert_idx;
        a->arrays_tail->next = nl;
        a->arrays_tail = nl;
    }
    a->indexes_types[a->insert_idx++] = T_ARR;
}

ArrayLink array_get(Array *a, int index)
{
    if (!a)
    {
        return (ArrayLink){ .type = T_ERROR };
    }

    StrArrLink *sl = 0;
    IntArrLink *il = 0;
    DoubleArrLink *dl = 0;
    BoolArrLink *bl = 0;
    NullArrLink *nl = 0;
    ArrArrLink *al = 0;
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
        return (ArrayLink){ .type = T_STR, .strv = sl->value };
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
        return (ArrayLink){ .type = T_INT, .integerv = il->value };
    case T_DOUBLE:
        dl = a->doubles_head;
        if (!dl)
        {
            return (ArrayLink){ .type = T_ERROR };
        }
        while (dl->next)
        {
            if (dl->index == index)
            {
                break;
            }
            dl = dl->next;
        }
        return (ArrayLink){ .type = T_DOUBLE, .doublev = dl->value };
    case T_BOOL:
        bl = a->booleans_head;
        if (!bl)
        {
            return (ArrayLink){ .type = T_ERROR };
        }
        while (bl->next)
        {
            if (bl->index == index)
            {
                break;
            }
            bl = bl->next;
        }
        return (ArrayLink){ .type = T_BOOL, .boolv = bl->value };
    case T_NULL:
        nl = a->nulls_head;
        if (!nl)
        {
            return (ArrayLink){ .type = T_ERROR };
        }
        while (nl->next)
        {
            if (nl->index == index)
            {
                break;
            }
            nl = nl->next;
        }
        return (ArrayLink){ .type = T_NULL };
    case T_ARR:
        al = a->arrays_head;
        if (!al)
        {
            return (ArrayLink){ .type = T_ERROR };
        }
        while (al->next)
        {
            if (al->index == index)
            {
                break;
            }
            al = al->next;
        }
        return (ArrayLink){ .type = T_ARR, .arrayv = al->value };
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

    DoubleArrLink *dl = a->doubles_head;
    while (dl)
    {
        DoubleArrLink *tmp = dl;
        dl = dl->next;
        free(tmp);
    }

    BoolArrLink *bl = a->booleans_head;
    while (bl)
    {
        BoolArrLink *tmp = bl;
        bl = bl->next;
        free(tmp);
    }

    NullArrLink *nl = a->nulls_head;
    while (nl)
    {
        NullArrLink *tmp = nl;
        nl = nl->next;
        free(tmp);
    }

    ArrArrLink *al = a->arrays_head;
    while (al)
    {
        ArrArrLink *tmp = al;
        al = al->next;
        free(tmp);
    }
}
