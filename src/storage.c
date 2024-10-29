#include "storage.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdlib.h>

/*******************************************************************************
**                                   MACROS                                   **
*******************************************************************************/
#define ERROR_VALUE ((Value){ .type = T_ERROR })
#define ERROR_ITEM ((Item){ .type = T_ERROR })

/**
** \def Common code to all the arr_add_<type>() functions
**      The null type cannot use this macro as there is no value to set
** \param type_head Can be one of the following :
**                  strings_head, integers_head, doubles_head, booleans_head,
**                  arrays_head, dicts_head
** \param type_tail Can be one of the following :
**                  strings_tail, integers_tail, doubles_tail, booleans_tail,
**                  arrays_tail, dicts_tail
** \param TypeArrLink Can be one of the following :
**                    StrArrLink, IntArrLink, DoubleArrLink, BoolArrLink,
**                    ArrArrLink, DictArrLink
** \param T_TYPE Can be one of the following :
**               T_STR, T_INT, T_DOUBLE, T_BOOL, T_ARR, T_DICT
*/
#define ARRAY_ADD(type_head, type_tail, TypeArrLink, T_TYPE)                   \
    if (!a->type_head)                                                         \
    {                                                                          \
        TypeArrLink *l = calloc(1, sizeof(TypeArrLink));                       \
        if (!l)                                                                \
        {                                                                      \
            return;                                                            \
        }                                                                      \
        l->value = value;                                                      \
        /* Sets the index of the element we are adding to the index of the     \
         * element to be added in the linked list */                           \
        l->index = a->insert_idx;                                              \
        /* As there was no element, the tail is the head */                    \
        a->type_head = l;                                                      \
        a->type_tail = l;                                                      \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        /* Goes to the last element of the linked list and adds the new        \
         * element */                                                          \
        TypeArrLink *nl = calloc(1, sizeof(TypeArrLink));                      \
        if (!nl)                                                               \
        {                                                                      \
            return;                                                            \
        }                                                                      \
        nl->value = value;                                                     \
        /* Sets the index of the element we are adding to the index of the     \
         * element to be added in the linked list */                           \
        nl->index = a->insert_idx;                                             \
        /* Sets the new tail to the element we are adding and make the         \
         * previous tail point to it */                                        \
        a->type_tail->next = nl;                                               \
        a->type_tail = nl;                                                     \
    }                                                                          \
    /* Adds to the 'indexes_types' list the type of the element of index       \
     * a->insert_idx */                                                        \
    a->indexes_types[a->insert_idx++] = T_TYPE

/**
** \def Common code to all the dict_add_<type>() functions
**      The null type cannot use this macro as there is no value to set
** \param type_head Can be one of the following :
**                  strings_head, integers_head, doubles_head, booleans_head,
**                  arrays_head, dicts_head
** \param type_tail Can be one of the following :
**                  strings_tail, integers_tail, doubles_tail, booleans_tail,
**                  arrays_tail, dicts_tail
** \param TypeDictLink Can be one of the following :
**                    StrDictLink, IntDictLink, DoubleDictLink, BoolDictLink,
**                    ArrDictLink, DictDictLink
** \param T_TYPE Can be one of the following :
**               T_STR, T_INT, T_DOUBLE, T_BOOL, T_ARR, T_DICT
*/
#define DICT_ADD(type_head, type_tail, TypeDictLink, T_TYPE)                   \
    if (!d->type_head)                                                         \
    {                                                                          \
        TypeDictLink *l = calloc(1, sizeof(TypeDictLink));                     \
        if (!l)                                                                \
        {                                                                      \
            return;                                                            \
        }                                                                      \
        l->value = value;                                                      \
        l->key = key;                                                          \
        d->type_head = l;                                                      \
        d->type_tail = l;                                                      \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        /* Goes to the last element of the linked list and adds the new        \
         * element*/                                                           \
        TypeDictLink *nl = calloc(1, sizeof(TypeDictLink));                    \
        if (!nl)                                                               \
        {                                                                      \
            return;                                                            \
        }                                                                      \
        nl->value = value;                                                     \
        nl->key = key;                                                         \
        d->type_tail->next = nl;                                               \
        d->type_tail = nl;                                                     \
    }                                                                          \
    d->keys_types[d->insert_idx++] = (KeyType)                                 \
    {                                                                          \
        .key = key, .type = T_TYPE                                             \
    }

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
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
    if (is_array)
    {
        j->array = a;
    }
    else
    {
        j->dict = d;
    }
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
    ARRAY_ADD(strings_head, strings_tail, StrArrLink, T_STR);
}

void arr_add_int(Array *a, int value)
{
    if (!a)
    {
        return;
    }
    ARRAY_ADD(integers_head, integers_tail, IntArrLink, T_INT);
}

void arr_add_double(Array *a, double value)
{
    if (!a)
    {
        return;
    }
    ARRAY_ADD(doubles_head, doubles_tail, DoubleArrLink, T_DOUBLE);
}

void arr_add_bool(Array *a, char value)
{
    if (!a)
    {
        return;
    }
    ARRAY_ADD(booleans_head, booleans_tail, BoolArrLink, T_BOOL);
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
    ARRAY_ADD(arrays_head, arrays_tail, ArrArrLink, T_ARR);
}

void arr_add_dict(Array *a, Dict *value)
{
    if (!a || !value)
    {
        return;
    }
    ARRAY_ADD(dicts_head, dicts_tail, DictArrLink, T_DICT);
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
    DICT_ADD(strings_head, strings_tail, StrDictLink, T_STR);
}

void dict_add_int(Dict *d, String key, int value)
{
    if (!d)
    {
        return;
    }
    DICT_ADD(integers_head, integers_tail, IntDictLink, T_INT);
}

void dict_add_double(Dict *d, String key, double value)
{
    if (!d)
    {
        return;
    }
    DICT_ADD(doubles_head, doubles_tail, DoubleDictLink, T_DOUBLE);
}

void dict_add_bool(Dict *d, String key, char value)
{
    if (!d)
    {
        return;
    }
    DICT_ADD(booleans_head, booleans_tail, BoolDictLink, T_BOOL);
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
    DICT_ADD(arrays_head, arrays_tail, ArrDictLink, T_ARR);
}

void dict_add_dict(Dict *d, String key, Dict *value)
{
    if (!d || !value)
    {
        return;
    }
    DICT_ADD(dicts_head, dicts_tail, DictDictLink, T_DICT);
}

/*******************************************************************************
**                                    GETS                                    **
*******************************************************************************/
Value array_get(Array *a, int index)
{
    if (!a)
    {
        return ERROR_VALUE;
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
        while (sl)
        {
            if (sl->index == index)
            {
                return (Value){ .type = T_STR, .strv = sl->value };
            }
            sl = sl->next;
        }
        break;
    case T_INT:
        il = a->integers_head;
        while (il)
        {
            if (il->index == index)
            {
                return (Value){ .type = T_INT, .intv = il->value };
            }
            il = il->next;
        }
        break;
    case T_DOUBLE:
        dl = a->doubles_head;
        while (dl)
        {
            if (dl->index == index)
            {
                return (Value){ .type = T_DOUBLE, .doublev = dl->value };
            }
            dl = dl->next;
        }
        break;
    case T_BOOL:
        bl = a->booleans_head;
        while (bl)
        {
            if (bl->index == index)
            {
                return (Value){ .type = T_BOOL, .boolv = bl->value };
            }
            bl = bl->next;
        }
        break;
    case T_NULL:
        nl = a->nulls_head;
        while (nl)
        {
            if (nl->index == index)
            {
                return (Value){ .type = T_NULL };
            }
            nl = nl->next;
        }
        break;
    case T_ARR:
        al = a->arrays_head;
        while (al)
        {
            if (al->index == index)
            {
                return (Value){ .type = T_ARR, .arrayv = al->value };
            }
            al = al->next;
        }
        break;
    case T_DICT:
        l = a->dicts_head;
        while (l)
        {
            if (l->index == index)
            {
                return (Value){ .type = T_DICT, .dictv = l->value };
            }
            l = l->next;
        }
        break;
    }
    return ERROR_VALUE;
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

Item dict_get(Dict *d, String key)
{
    if (!d)
    {
        return ERROR_ITEM;
    }

    int index = get_key_index(d->keys_types, key, d->size);
    if (index < 0)
    {
        return ERROR_ITEM;
    }

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
        while (sl)
        {
            if (strings_equals(key, sl->key))
            {
                return (Item){ .type = T_STR, .key = key, .strv = sl->value };
            }
            sl = sl->next;
        }
        break;
    case T_INT:
        il = d->integers_head;
        while (il)
        {
            if (strings_equals(key, il->key))
            {
                return (Item){ .type = T_INT, .key = key, .intv = il->value };
            }
            il = il->next;
        }
        break;
    case T_DOUBLE:
        dl = d->doubles_head;
        while (dl)
        {
            if (strings_equals(key, dl->key))
            {
                return (
                    Item){ .type = T_DOUBLE, .key = key, .doublev = dl->value };
            }
            dl = dl->next;
        }
        break;
    case T_BOOL:
        bl = d->booleans_head;
        while (bl)
        {
            if (strings_equals(key, bl->key))
            {
                return (Item){ .type = T_BOOL, .key = key, .boolv = bl->value };
            }
            bl = bl->next;
        }
        break;
    case T_NULL:
        nl = d->nulls_head;
        while (nl)
        {
            if (strings_equals(key, nl->key))
            {
                return (Item){ .type = T_NULL, .key = key };
            }
            nl = nl->next;
        }
        break;
    case T_ARR:
        al = d->arrays_head;
        while (al)
        {
            if (strings_equals(key, al->key))
            {
                return (Item){ .type = T_ARR, .key = key, .arrayv = al->value };
            }
            al = al->next;
        }
        break;
    case T_DICT:
        l = d->dicts_head;
        while (l)
        {
            if (strings_equals(key, l->key))
            {
                return (Item){ .type = T_DICT, .key = key, .dictv = l->value };
            }
            l = l->next;
        }
        break;
    }
    return ERROR_ITEM;
}

/*******************************************************************************
**                                   DESTROYS                                 **
*******************************************************************************/
void destroy_array(Array *a)
{
    if (!a)
    {
        return;
    }

    free(a->indexes_types);

    StrArrLink *sl = a->strings_head;
    while (sl)
    {
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
        free(tmp);
    }

    DictArrLink *l = a->dicts_head;
    while (l)
    {
        DictArrLink *tmp = l;
        l = l->next;
        destroy_dict(tmp->value);
        free(tmp);
    }

    free(a);
}

void destroy_dict(Dict *d)
{
    if (!d)
    {
        return;
    }

    // The keys that are pointed to by the elements of this array will be freed
    // when freeing the elements, so we don't do it here
    free(d->keys_types);

    StrDictLink *sl = d->strings_head;
    while (sl)
    {
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
        free(tmp);
    }

    DictDictLink *l = d->dicts_head;
    while (l)
    {
        DictDictLink *tmp = l;
        l = l->next;
        free(tmp->key.str);
        destroy_dict(tmp->value);
        free(tmp);
    }

    free(d);
}

void destroy_json(JSON *j)
{
    if (!j)
    {
        return;
    }

    if (j->is_array)
    {
        destroy_array(j->array);
    }
    else
    {
        destroy_dict(j->dict);
    }

    free(j);
}
