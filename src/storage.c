#include "storage.h"

#include <stdlib.h>

Array *init_array(int size)
{
    char *indexes_types = calloc(size, sizeof(char));
    if (!indexes_types)
    {
        size = 0;
    }
    Array *a = calloc(1, sizeof(Array));
    if (!a)
    {
        return 0;
    }
    a->size = size;
    a->indexes_types = indexes_types;
    return a;
}

Dict *init_dict(int size)
{
    KeyType *keys_types = calloc(size, sizeof(KeyType));
    if (!keys_types)
    {
        size = 0;
    }
    Dict *d = calloc(1, sizeof(Dict));
    if (!d)
    {
        return 0;
    }
    d->size = size;
    d->keys_types = keys_types;
    return d;
}

JSON *init_json(char is_array, Array *a, Dict *d)
{
    JSON *j = calloc(1, sizeof(JSON));
    if (!j)
    {
        return 0;
    }
    j->is_array = is_array;
    j->array = a;
    j->dict = d;
    return j;
}

/*******************************************************************************
**                                 ARRAYS ADDS                                **
*******************************************************************************/
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

void arr_add_arr(Array *a, Array *value)
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

void arr_add_dict(Array *a, Dict *value)
{
    if (!a || !value)
    {
        return;
    }

    if (!a->dicts_head)
    {
        DictArrLink *l = calloc(1, sizeof(DictArrLink));
        if (!l)
        {
            return;
        }
        l->value = value;
        l->index = a->insert_idx;
        a->dicts_head = l;
        a->dicts_tail = l;
    }
    else
    {
        // Goes to the last element of the linked list and adds the new element
        DictArrLink *nl = calloc(1, sizeof(DictArrLink));
        if (!nl)
        {
            return;
        }
        nl->value = value;
        nl->index = a->insert_idx;
        a->dicts_tail->next = nl;
        a->dicts_tail = nl;
    }
    a->indexes_types[a->insert_idx++] = T_DICT;
}

/*******************************************************************************
**                                 DICTS ADDS                                 **
*******************************************************************************/
void dict_add_str(Dict *d, String key, String value)
{
    if (!d)
    {
        return;
    }

    if (!d->strings_head)
    {
        StrDictLink *l = calloc(1, sizeof(StrDictLink));
        if (!l)
        {
            return;
        }
        l->value = value;
        l->key = key;
        d->strings_head = l;
        d->strings_tail = l;
    }
    else
    {
        // Goes to the last element of the linked list and adds the new element
        StrDictLink *nl = calloc(1, sizeof(StrDictLink));
        if (!nl)
        {
            return;
        }

        nl->value = value;
        nl->key = key;
        d->strings_tail->next = nl;
        d->strings_tail = nl;
    }
    d->keys_types[d->insert_idx++] = (KeyType){ .key = key, .type = T_STR };
}

void dict_add_int(Dict *d, String key, int value)
{
    if (!d)
    {
        return;
    }

    if (!d->integers_head)
    {
        IntDictLink *l = calloc(1, sizeof(IntDictLink));
        if (!l)
        {
            return;
        }
        l->value = value;
        l->key = key;
        d->integers_head = l;
        d->integers_tail = l;
    }
    else
    {
        // Goes to the last element of the linked list and adds the new element
        IntDictLink *nl = calloc(1, sizeof(IntDictLink));
        if (!nl)
        {
            return;
        }
        nl->value = value;
        nl->key = key;
        d->integers_tail->next = nl;
        d->integers_tail = nl;
    }
    d->keys_types[d->insert_idx++] = (KeyType){ .key = key, .type = T_INT };
}

void dict_add_double(Dict *d, String key, double value)
{
    if (!d)
    {
        return;
    }

    if (!d->doubles_head)
    {
        DoubleDictLink *l = calloc(1, sizeof(DoubleDictLink));
        if (!l)
        {
            return;
        }
        l->value = value;
        l->key = key;
        d->doubles_head = l;
        d->doubles_tail = l;
    }
    else
    {
        // Goes to the last element of the linked list and adds the new element
        DoubleDictLink *nl = calloc(1, sizeof(DoubleDictLink));
        if (!nl)
        {
            return;
        }
        nl->value = value;
        nl->key = key;
        d->doubles_tail->next = nl;
        d->doubles_tail = nl;
    }
    d->keys_types[d->insert_idx++] = (KeyType){ .key = key, .type = T_DOUBLE };
}

void dict_add_bool(Dict *d, String key, char value)
{
    if (!d)
    {
        return;
    }

    if (!d->booleans_head)
    {
        BoolDictLink *l = calloc(1, sizeof(BoolDictLink));
        if (!l)
        {
            return;
        }
        l->value = value;
        l->key = key;
        d->booleans_head = l;
        d->booleans_tail = l;
    }
    else
    {
        // Goes to the last element of the linked list and adds the new element
        BoolDictLink *nl = calloc(1, sizeof(BoolDictLink));
        if (!nl)
        {
            return;
        }
        nl->value = value;
        nl->key = key;
        d->booleans_tail->next = nl;
        d->booleans_tail = nl;
    }
    d->keys_types[d->insert_idx++] = (KeyType){ .key = key, .type = T_BOOL };
}

void dict_add_null(Dict *d, String key)
{
    if (!d)
    {
        return;
    }

    if (!d->nulls_head)
    {
        NullDictLink *l = calloc(1, sizeof(NullDictLink));
        if (!l)
        {
            return;
        }
        l->key = key;
        d->nulls_head = l;
        d->nulls_tail = l;
    }
    else
    {
        // Goes to the last element of the linked list and adds the new element
        NullDictLink *nl = calloc(1, sizeof(NullDictLink));
        if (!nl)
        {
            return;
        }
        nl->key = key;
        d->nulls_tail->next = nl;
        d->nulls_tail = nl;
    }
    d->keys_types[d->insert_idx++] = (KeyType){ .key = key, .type = T_NULL };
}

void dict_add_arr(Dict *d, String key, Array *value)
{
    if (!d || !value)
    {
        return;
    }

    if (!d->arrays_head)
    {
        ArrDictLink *l = calloc(1, sizeof(ArrDictLink));
        if (!l)
        {
            return;
        }
        l->value = value;
        l->key = key;
        d->arrays_head = l;
        d->arrays_tail = l;
    }
    else
    {
        // Goes to the last element of the linked list and adds the new element
        ArrDictLink *nl = calloc(1, sizeof(ArrDictLink));
        if (!nl)
        {
            return;
        }
        nl->value = value;
        nl->key = key;
        d->arrays_tail->next = nl;
        d->arrays_tail = nl;
    }
    d->keys_types[d->insert_idx++] = (KeyType){ .key = key, .type = T_ARR };
}

void dict_add_dict(Dict *d, String key, Dict *value)
{
    if (!d || !value)
    {
        return;
    }

    if (!d->dicts_head)
    {
        DictDictLink *l = calloc(1, sizeof(DictDictLink));
        if (!l)
        {
            return;
        }
        l->value = value;
        l->key = key;
        d->dicts_head = l;
        d->dicts_tail = l;
    }
    else
    {
        // Goes to the last element of the linked list and adds the new element
        DictDictLink *nl = calloc(1, sizeof(DictDictLink));
        if (!nl)
        {
            return;
        }
        nl->value = value;
        nl->key = key;
        d->dicts_tail->next = nl;
        d->dicts_tail = nl;
    }
    d->keys_types[d->insert_idx++] = (KeyType){ .key = key, .type = T_DICT };
}

/*******************************************************************************
**                                    GETS                                    **
*******************************************************************************/
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
    DictArrLink *l = 0;
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
    case T_DICT:
        l = a->dicts_head;
        if (!l)
        {
            return (ArrayLink){ .type = T_ERROR };
        }
        while (l->next)
        {
            if (l->index == index)
            {
                break;
            }
            l = l->next;
        }
        return (ArrayLink){ .type = T_DICT, .dictv = l->value };
    }
    return (ArrayLink){ .type = T_ERROR };
}

int get_key_index(KeyType *kt, String key, int nb_keys)
{
    if (!kt)
    {
        return -1;
    }

    for (int i = 0; i < nb_keys; ++i)
    {
        if (strings_equals(key, kt[i].key))
        {
            return i;
        }
    }
    return -1;
}

DictLink dict_get(Dict *d, String key)
{
    if (!d)
    {
        return (DictLink){ .type = T_ERROR };
    }

    int index = get_key_index(d->keys_types, key, d->size);

    StrDictLink *sl = 0;
    IntDictLink *il = 0;
    DoubleDictLink *dl = 0;
    BoolDictLink *bl = 0;
    NullDictLink *nl = 0;
    ArrDictLink *al = 0;
    DictDictLink *l = 0;
    switch (d->keys_types[index].type)
    {
    case T_STR:
        sl = d->strings_head;
        if (!sl)
        {
            return (DictLink){ .type = T_ERROR };
        }
        while (sl->next)
        {
            if (strings_equals(key, sl->key))
            {
                break;
            }
            sl = sl->next;
        }
        return (DictLink){ .type = T_STR, .key = key, .strv = sl->value };
    case T_INT:
        il = d->integers_head;
        if (!il)
        {
            return (DictLink){ .type = T_ERROR };
        }
        while (il->next)
        {
            if (strings_equals(key, il->key))
            {
                break;
            }
            il = il->next;
        }
        return (DictLink){ .type = T_INT, .key = key, .integerv = il->value };
    case T_DOUBLE:
        dl = d->doubles_head;
        if (!dl)
        {
            return (DictLink){ .type = T_ERROR };
        }
        while (dl->next)
        {
            if (strings_equals(key, dl->key))
            {
                break;
            }
            dl = dl->next;
        }
        return (DictLink){ .type = T_DOUBLE, .key = key, .doublev = dl->value };
    case T_BOOL:
        bl = d->booleans_head;
        if (!bl)
        {
            return (DictLink){ .type = T_ERROR };
        }
        while (bl->next)
        {
            if (strings_equals(key, bl->key))
            {
                break;
            }
            bl = bl->next;
        }
        return (DictLink){ .type = T_BOOL, .key = key, .boolv = bl->value };
    case T_NULL:
        nl = d->nulls_head;
        if (!nl)
        {
            return (DictLink){ .type = T_ERROR };
        }
        while (nl->next)
        {
            if (strings_equals(key, nl->key))
            {
                break;
            }
            nl = nl->next;
        }
        return (DictLink){ .type = T_NULL, .key = key };
    case T_ARR:
        al = d->arrays_head;
        if (!al)
        {
            return (DictLink){ .type = T_ERROR };
        }
        while (al->next)
        {
            if (strings_equals(key, al->key))
            {
                break;
            }
            al = al->next;
        }
        return (DictLink){ .type = T_ARR, .key = key, .arrayv = al->value };
    case T_DICT:
        l = d->dicts_head;
        if (!l)
        {
            return (DictLink){ .type = T_ERROR };
        }
        while (l->next)
        {
            if (strings_equals(key, l->key))
            {
                break;
            }
            l = l->next;
        }
        return (DictLink){ .type = T_DICT, .key = key, .dictv = l->value };
    }
    return (DictLink){ .type = T_ERROR };
}

/*******************************************************************************
**                                   DESTROYS                                 **
*******************************************************************************/
void destroy_array(Array *a)
{
    free(a->indexes_types);

    StrArrLink *sl = a->strings_head;
    while (sl)
    {
        // TODO: Free strings, as they normally are dynamically allocated
        StrArrLink *tmp = sl;
        sl = sl->next;
        free(tmp->value.str);
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
        destroy_array(tmp->value);
        free(tmp->value);
        free(tmp);
    }

    DictArrLink *l = a->dicts_head;
    while (l)
    {
        DictArrLink *tmp = l;
        l = l->next;
        destroy_dict(tmp->value);
        free(tmp->value);
        free(tmp);
    }
}

void destroy_dict(Dict *d)
{
    // The keys that are pointed to by the elements of this array will be freed
    // when freeing the element, so we don't do it here
    free(d->keys_types);

    StrDictLink *sl = d->strings_head;
    while (sl)
    {
        // TODO: Free strings, as they normally are dynamically allocated
        StrDictLink *tmp = sl;
        sl = sl->next;
        free(tmp->key.str);
        free(tmp->value.str);
        free(tmp);
    }

    IntDictLink *il = d->integers_head;
    while (il)
    {
        IntDictLink *tmp = il;
        il = il->next;
        free(tmp->key.str);
        free(tmp);
    }

    DoubleDictLink *dl = d->doubles_head;
    while (dl)
    {
        DoubleDictLink *tmp = dl;
        dl = dl->next;
        free(tmp->key.str);
        free(tmp);
    }

    BoolDictLink *bl = d->booleans_head;
    while (bl)
    {
        BoolDictLink *tmp = bl;
        bl = bl->next;
        free(tmp->key.str);
        free(tmp);
    }

    NullDictLink *nl = d->nulls_head;
    while (nl)
    {
        NullDictLink *tmp = nl;
        nl = nl->next;
        free(tmp->key.str);
        free(tmp);
    }

    ArrDictLink *al = d->arrays_head;
    while (al)
    {
        ArrDictLink *tmp = al;
        al = al->next;
        free(tmp->key.str);
        destroy_array(tmp->value);
        free(tmp->value);
        free(tmp);
    }

    DictDictLink *l = d->dicts_head;
    while (l)
    {
        DictDictLink *tmp = l;
        l = l->next;
        free(tmp->key.str);
        destroy_dict(tmp->value);
        free(tmp->value);
        free(tmp);
    }
}

void destroy_json(JSON *j)
{
    if (!j)
    {
        return;
    }

    if (j->array)
    {
        destroy_array(j->array);
    }

    if (j->dict)
    {
        destroy_dict(j->dict);
    }
}
