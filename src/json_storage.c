#include "json_storage.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdlib.h>

/*******************************************************************************
**                                   MACROS                                   **
*******************************************************************************/
#define ERROR_VALUE ((Value){ .type = T_ERROR })
#define ERROR_ITEM ((Item){ .type = T_ERROR })

#ifdef EDITING_MODE
/**
** \def Adds an element to the given array or dict
** \param TypeLink Can be either 'ValueLink' or 'ItemLink'
** \param x The name of the array or dict variable (a for array, d for dict)
*/
#    define ADD(TypeLink, x)                                                   \
        if (!x->head)                                                          \
        {                                                                      \
            TypeLink *l = calloc(1, sizeof(TypeLink));                         \
            if (!l)                                                            \
            {                                                                  \
                return;                                                        \
            }                                                                  \
            x->head = l;                                                       \
            x->tail = l;                                                       \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            if (!x->tail)                                                      \
            {                                                                  \
                return;                                                        \
            }                                                                  \
            /* If the current link's array is full */                          \
            if (x->tail->insert_index >= ARRAY_LEN)                            \
            {                                                                  \
                TypeLink *l = calloc(1, sizeof(TypeLink));                     \
                if (!l)                                                        \
                {                                                              \
                    return;                                                    \
                }                                                              \
                x->tail->next = l;                                             \
                x->tail = l;                                                   \
            }                                                                  \
        }                                                                      \
        ++x->size

/*******************************************************************************
**                               LOCAL FUNCTIONS                              **
*******************************************************************************/
#    ifdef DEBUG
#        include <stdio.h>
/**
** \brief Debug function that allows printing the linked lists arrays
*/
void arr_print_array(Array *a)
{
    if (!a)
    {
        return;
    }

    ValueLink *link = a->head;
    while (link)
    {
        printf("[");
        Value *values = link->values;
        for (unsigned i = 0; i < ARRAY_LEN; ++i)
        {
            if (values[i].type != 0)
            {
                Value v = values[i];
                switch (v.type)
                {
                case T_STR:
                    printf("\"%s\", ", v.strv.str ? v.strv.str : "");
                    break;
                case T_INT:
                    printf("%d, ", v.intv);
                    break;
                case T_DOUBLE:
                    printf("%f, ", v.doublev);
                    break;
                case T_BOOL:
                    printf("%s, ", v.boolv ? "true" : "false");
                    break;
                case T_NULL:
                    printf("null, ");
                    break;
                case T_ARR:
                    printf("array, ");
                    break;
                case T_DICT:
                    printf("dict, ");
                    break;
                }
            }
            else
            {
                printf(" , ");
            }
        }
        printf("]\n");
        link = link->next;
    }
    printf("\n");
}
#    endif // !DEBUG

void arr_copy_array(Value *src, Value *dest)
{
    if (!src || !dest)
    {
        return;
    }

    for (unsigned i = 0; i < ARRAY_LEN; ++i)
    {
        dest[i] = src[i];
    }
}

void dict_copy_array(Item *src, Item *dest)
{
    if (!src || !dest)
    {
        return;
    }

    for (unsigned i = 0; i < ARRAY_LEN; ++i)
    {
        dest[i] = src[i];
    }
}

void arr_empty_array(Value *array)
{
    if (!array)
    {
        return;
    }

    for (unsigned i = 0; i < ARRAY_LEN; ++i)
    {
        array[i].type = 0;
    }
}

void dict_empty_array(Item *array)
{
    if (!array)
    {
        return;
    }

    for (unsigned i = 0; i < ARRAY_LEN; ++i)
    {
        array[i].type = 0;
    }
}

void defragment_array(Array *a)
{
    if (!a)
    {
        return;
    }

    // We fill this array with each non-empty element (where the type is not
    // T_ERROR), and then we copy its contents to the previously fragmented
    // array
    Value tmps[ARRAY_LEN] = { 0 };
    unsigned tmps_insert_idx = 0;

    ValueLink *link_to_fill = a->head;
    ValueLink *prev_link_to_fill = a->head;
    char last_started_fill = 0;

    ValueLink *link = a->head;
    // We gather all the non empty values inside 'tmps', and then we copy the
    // contents of 'tmps' in the 'link_to_fill' (which starts at the head)
    while (link)
    {
        for (unsigned i = 0; i < ARRAY_LEN; ++i)
        {
            // The temp array is filled, so we can put the defragmented array
            // back inside it
            if (tmps_insert_idx == ARRAY_LEN)
            {
                // Copies the temp array to the base Array
                arr_copy_array(tmps, link_to_fill->values);
                link_to_fill->insert_index = ARRAY_LEN;
                arr_empty_array(tmps);
                tmps_insert_idx = 0;
                prev_link_to_fill = link_to_fill;
                link_to_fill = link_to_fill->next;
                last_started_fill = 0;
            }

            if (link->values[i].type != T_ERROR)
            {
                tmps[tmps_insert_idx++] = link->values[i];
                last_started_fill = 1;
            }
        }
        link = link->next;
    }

    // When the last link's array didn't reach the max number of contained
    // elements, we add the remaining elements and free all the remaining unused
    // links
    if (tmps_insert_idx)
    {
        arr_copy_array(tmps, link_to_fill->values);
        a->tail = link_to_fill;
        link_to_fill = link_to_fill->next;
        while (link_to_fill)
        {
            ValueLink *tmp = link_to_fill;
            link_to_fill = link_to_fill->next;
            free(tmp);
        }
        a->tail->next = 0;
    }
    // If we filled the array in the linked list but there is no value left
    // after, it means that the 'link_to_fill' now points to a link that we have
    // to remove, because the defragmentation is done so it is no longer the
    // link to fill.
    // This is why we make the tail point to the previous one, and free all the
    // remaining links starting at 'link_to_fill'
    else if (!last_started_fill)
    {
        a->tail = prev_link_to_fill;
        a->tail->next = 0;
        while (link_to_fill)
        {
            ValueLink *tmp = link_to_fill;
            link_to_fill = link_to_fill->next;
            free(tmp);
        }
    }

    // Sets the insert index of the last link to the correct position
    Value *values = a->tail->values;
    for (unsigned i = 0; i < ARRAY_LEN; ++i)
    {
        if (values[i].type == T_ERROR)
        {
            a->tail->insert_index = i;
            break;
        }
    }
}

void defragment_dict(Dict *d)
{
    if (!d)
    {
        return;
    }

    // We fill this array with each non-empty element (where the type is not
    // T_ERROR), and then we copy its contents to the previously fragmented
    // array
    Item tmps[ARRAY_LEN] = { 0 };
    unsigned tmps_insert_idx = 0;

    ItemLink *link_to_fill = d->head;
    ItemLink *prev_link_to_fill = d->head;
    char last_started_fill = 0;

    ItemLink *link = d->head;
    // We gather all the non empty items inside 'tmps', and then we copy the
    // contents of 'tmps' in the 'link_to_fill' (which starts at the head)
    while (link)
    {
        for (unsigned i = 0; i < ARRAY_LEN; ++i)
        {
            // The temp array is filled, so we can put the defragmented array
            // back inside it
            if (tmps_insert_idx == ARRAY_LEN)
            {
                // Copies the temp array to the base Dict
                dict_copy_array(tmps, link_to_fill->items);
                link_to_fill->insert_index = ARRAY_LEN;
                dict_empty_array(tmps);
                tmps_insert_idx = 0;
                prev_link_to_fill = link_to_fill;
                link_to_fill = link_to_fill->next;
                last_started_fill = 0;
            }

            if (link->items[i].type != T_ERROR)
            {
                tmps[tmps_insert_idx++] = link->items[i];
                last_started_fill = 1;
            }
        }
        link = link->next;
    }

    // When the last link's array didn't reach the max number of contained
    // elements, we add the remaining elements and free all the remaining unused
    // links
    if (tmps_insert_idx)
    {
        dict_copy_array(tmps, link_to_fill->items);
        d->tail = link_to_fill;
        link_to_fill = link_to_fill->next;
        while (link_to_fill)
        {
            ItemLink *tmp = link_to_fill;
            link_to_fill = link_to_fill->next;
            free(tmp);
        }
        d->tail->next = 0;
    }
    // If we filled the array in the linked list but there is no value left
    // after, it means that the 'link_to_fill' now points to a link that we have
    // to remove, because the defragmentation is done so it is no longer the
    // link to fill.
    // This is why we make the tail point to the previous one, and free all the
    // remaining links starting at 'link_to_fill'
    else if (!last_started_fill)
    {
        d->tail = prev_link_to_fill;
        d->tail->next = 0;
        while (link_to_fill)
        {
            ItemLink *tmp = link_to_fill;
            link_to_fill = link_to_fill->next;
            free(tmp);
        }
    }

    // Sets the insert index of the last link to the correct position
    Item *items = d->tail->items;
    for (unsigned i = 0; i < ARRAY_LEN; ++i)
    {
        if (items[i].type == T_ERROR)
        {
            d->tail->insert_index = i;
            break;
        }
    }
}
#else
/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
char strings_equals(String s1, String s2)
{
    unsigned length = s1.length;
    if (length != s2.length)
    {
        return 0;
    }

    const char *a = s1.str;
    const char *b = s2.str;
    if (!a || !b)
    {
        return 0;
    }

    for (unsigned i = 0; i < length; ++i)
    {
        if (a[i] != b[i])
        {
            return 0;
        }
    }
    return 1;
}

Array *init_array(unsigned size)
{
    Array *a = calloc(1, sizeof(Array));
    if (!a)
    {
        return 0;
    }

    if (!size)
    {
        return a;
    }

    a->values = calloc(size, sizeof(Value));
    if (!a->values)
    {
        free(a);
        return 0;
    }
    a->size = size;
    return a;
}

Dict *init_dict(unsigned size)
{
    Dict *d = calloc(1, sizeof(Dict));
    if (!d)
    {
        return 0;
    }

    if (!size)
    {
        return d;
    }

    d->items = calloc(size, sizeof(Item));
    if (!d->items)
    {
        free(d);
        return 0;
    }
    d->size = size;
    return d;
}
#endif // !EDITING_MODE

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
**                                     ADDS                                   **
*******************************************************************************/
#ifndef EDITING_MODE
void arr_add_str(Array *a, String value)
{
    if (a && a->values && a->insert_index < a->size)
    {
        a->values[a->insert_index++] = (Value){ .type = T_STR, .strv = value };
    }
}

void arr_add_int(Array *a, int value)
{
    if (a && a->values && a->insert_index < a->size)
    {
        a->values[a->insert_index++] = (Value){ .type = T_INT, .intv = value };
    }
}

void arr_add_double(Array *a, double value)
{
    if (a && a->values && a->insert_index < a->size)
    {
        a->values[a->insert_index++] =
            (Value){ .type = T_DOUBLE, .doublev = value };
    }
}

void arr_add_bool(Array *a, char value)
{
    if (a && a->values && a->insert_index < a->size)
    {
        a->values[a->insert_index++] =
            (Value){ .type = T_BOOL, .boolv = value };
    }
}

void arr_add_null(Array *a)
{
    if (a && a->values && a->insert_index < a->size)
    {
        a->values[a->insert_index++] = (Value){ .type = T_NULL };
    }
}

void arr_add_arr(Array *a, Array *value)
{
    if (a && a->values && value && a->insert_index < a->size)
    {
        a->values[a->insert_index++] =
            (Value){ .type = T_ARR, .arrayv = value };
    }
}

void arr_add_dict(Array *a, Dict *value)
{
    if (a && a->values && value && a->insert_index < a->size)
    {
        a->values[a->insert_index++] =
            (Value){ .type = T_DICT, .dictv = value };
    }
}

void dict_add_str(Dict *d, String key, String value)
{
    if (d && d->items && d->insert_index < d->size)
    {
        d->items[d->insert_index++] =
            (Item){ .type = T_STR, .key = key, .strv = value };
    }
}

void dict_add_int(Dict *d, String key, int value)
{
    if (d && d->items && d->insert_index < d->size)
    {
        d->items[d->insert_index++] =
            (Item){ .type = T_INT, .key = key, .intv = value };
    }
}

void dict_add_double(Dict *d, String key, double value)
{
    if (d && d->items && d->insert_index < d->size)
    {
        d->items[d->insert_index++] =
            (Item){ .type = T_DOUBLE, .key = key, .doublev = value };
    }
}

void dict_add_bool(Dict *d, String key, char value)
{
    if (d && d->items && d->insert_index < d->size)
    {
        d->items[d->insert_index++] =
            (Item){ .type = T_BOOL, .key = key, .boolv = value };
    }
}

void dict_add_null(Dict *d, String key)
{
    if (d && d->items && d->insert_index < d->size)
    {
        d->items[d->insert_index++] = (Item){ .type = T_NULL, .key = key };
    }
}

void dict_add_arr(Dict *d, String key, Array *value)
{
    if (d && d->items && value && d->insert_index < d->size)
    {
        d->items[d->insert_index++] =
            (Item){ .type = T_ARR, .key = key, .arrayv = value };
    }
}

void dict_add_dict(Dict *d, String key, Dict *value)
{
    if (d && d->items && value && d->insert_index < d->size)
    {
        d->items[d->insert_index++] =
            (Item){ .type = T_DICT, .key = key, .dictv = value };
    }
}
#else
void arr_add_str(Array *a, String value)
{
    if (a)
    {
        ADD(ValueLink, a);
        a->tail->values[a->tail->insert_index++] =
            (Value){ .type = T_STR, .strv = value };
    }
}

void arr_add_int(Array *a, int value)
{
    if (a)
    {
        ADD(ValueLink, a);
        a->tail->values[a->tail->insert_index++] =
            (Value){ .type = T_INT, .intv = value };
    }
}

void arr_add_double(Array *a, double value)
{
    if (a)
    {
        ADD(ValueLink, a);
        a->tail->values[a->tail->insert_index++] =
            (Value){ .type = T_DOUBLE, .doublev = value };
    }
}

void arr_add_bool(Array *a, char value)
{
    if (a)
    {
        ADD(ValueLink, a);
        a->tail->values[a->tail->insert_index++] =
            (Value){ .type = T_BOOL, .boolv = value };
    }
}

void arr_add_null(Array *a)
{
    if (a)
    {
        ADD(ValueLink, a);
        a->tail->values[a->tail->insert_index++] = (Value){ .type = T_NULL };
    }
}

void arr_add_arr(Array *a, Array *value)
{
    if (a && value)
    {
        ADD(ValueLink, a);
        a->tail->values[a->tail->insert_index++] =
            (Value){ .type = T_ARR, .arrayv = value };
    }
}

void arr_add_dict(Array *a, Dict *value)
{
    if (a && value)
    {
        ADD(ValueLink, a);
        a->tail->values[a->tail->insert_index++] =
            (Value){ .type = T_DICT, .dictv = value };
    }
}

void dict_add_str(Dict *d, String key, String value)
{
    if (d)
    {
        ADD(ItemLink, d);
        d->tail->items[d->tail->insert_index++] =
            (Item){ .type = T_STR, .key = key, .strv = value };
    }
}

void dict_add_int(Dict *d, String key, int value)
{
    if (d)
    {
        ADD(ItemLink, d);
        d->tail->items[d->tail->insert_index++] =
            (Item){ .type = T_INT, .key = key, .intv = value };
    }
}

void dict_add_double(Dict *d, String key, double value)
{
    if (d)
    {
        ADD(ItemLink, d);
        d->tail->items[d->tail->insert_index++] =
            (Item){ .type = T_DOUBLE, .key = key, .doublev = value };
    }
}

void dict_add_bool(Dict *d, String key, char value)
{
    if (d)
    {
        ADD(ItemLink, d);
        d->tail->items[d->tail->insert_index++] =
            (Item){ .type = T_BOOL, .key = key, .boolv = value };
    }
}

void dict_add_null(Dict *d, String key)
{
    if (d)
    {
        ADD(ItemLink, d);
        d->tail->items[d->tail->insert_index++] =
            (Item){ .type = T_NULL, .key = key };
    }
}

void dict_add_arr(Dict *d, String key, Array *value)
{
    if (d && value)
    {
        ADD(ItemLink, d);
        d->tail->items[d->tail->insert_index++] =
            (Item){ .type = T_ARR, .key = key, .arrayv = value };
    }
}

void dict_add_dict(Dict *d, String key, Dict *value)
{
    if (d && value)
    {
        ADD(ItemLink, d);
        d->tail->items[d->tail->insert_index++] =
            (Item){ .type = T_DICT, .key = key, .dictv = value };
    }
}

/*******************************************************************************
**                                   REMOVES                                  **
*******************************************************************************/
void arr_remove(Array *a, unsigned index)
{
    if (!a || index >= a->size)
    {
        return;
    }

    ValueLink *link = a->head;
    unsigned non_null_values = 0;
    while (link)
    {
        for (unsigned i = 0; i < ARRAY_LEN; ++i)
        {
            Value v = link->values[i];
            if (v.type != T_ERROR)
            {
                ++non_null_values;
            }

            if (non_null_values && non_null_values - 1 == index)
            {
                switch (v.type)
                {
                case T_STR:
                    free(v.strv.str);
                    v.strv.str = 0;
                    break;
                case T_ARR:
                    destroy_array(v.arrayv);
                    v.arrayv = 0;
                    break;
                case T_DICT:
                    destroy_dict(v.dictv);
                    v.dictv = 0;
                    break;
                };
                link->values[i].type = T_ERROR;

                --a->size;
                ++a->nb_deletions;
                if (a->nb_deletions == NB_DELETIONS_TO_DEFRAG)
                {
                    defragment_array(a);
                    a->nb_deletions = 0;
                }
                return;
            }
        }
        link = link->next;
    }
}

void dict_remove(Dict *d, String key)
{
    if (!d || !key.str)
    {
        return;
    }

    ItemLink *link = d->head;
    while (link)
    {
        for (unsigned i = 0; i < ARRAY_LEN; ++i)
        {
            Item it = link->items[i];
            if (it.type != T_ERROR && strings_equals(key, it.key))
            {
                switch (it.type)
                {
                case T_STR:
                    free(it.strv.str);
                    it.strv.str = 0;
                    break;
                case T_ARR:
                    destroy_array(it.arrayv);
                    it.arrayv = 0;
                    break;
                case T_DICT:
                    destroy_dict(it.dictv);
                    it.dictv = 0;
                    break;
                };
                free(it.key.str);
                it.key.str = 0;
                link->items[i].type = T_ERROR;

                --d->size;
                ++d->nb_deletions;
                if (d->nb_deletions == NB_DELETIONS_TO_DEFRAG)
                {
                    defragment_dict(d);
                    d->nb_deletions = 0;
                }
                return;
            }
        }
        link = link->next;
    }
}
#endif // !EDITING_MODE

/*******************************************************************************
**                                    GETS                                    **
*******************************************************************************/
Value array_get(Array *a, unsigned index)
{
    if (!a || index >= a->size)
    {
        return ERROR_VALUE;
    }

#ifndef EDITING_MODE
    return index < a->size ? a->values[index] : ERROR_VALUE;
#else
    ValueLink *link = a->head;
    unsigned non_null_values = 0;
    while (link)
    {
        for (unsigned i = 0; i < ARRAY_LEN; ++i)
        {
            if (non_null_values == index)
            {
                return link->values[i];
            }
            if (link->values[i].type != T_ERROR)
            {
                ++non_null_values;
            }
        }
        link = link->next;
    }
    return ERROR_VALUE;
#endif // !EDITING_MODE
}

Item dict_get(Dict *d, String key)
{
    if (!d)
    {
        return ERROR_ITEM;
    }

#ifndef EDITING_MODE
    Item *items = d->items;
    unsigned size = d->size;
    for (unsigned i = 0; i < size; ++i)
    {
        Item it = items[i];
        if (strings_equals(key, it.key))
        {
            return it;
        }
    }
#else
    ItemLink *link = d->head;
    unsigned non_null_values = 0;
    while (link)
    {
        for (unsigned i = 0; i < ARRAY_LEN; ++i)
        {
            Item it = link->items[i];
            if (strings_equals(key, it.key))
            {
                return it;
            }
            if (it.type != T_ERROR)
            {
                ++non_null_values;
            }
        }
        link = link->next;
    }
#endif // !EDITING_MODE
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

#ifndef EDITING_MODE
    Value *values = a->values;
    unsigned size = a->size;
    for (unsigned i = 0; i < size; ++i)
    {
        Value val = values[i];
        switch (val.type)
        {
        case T_STR:
            free(val.strv.str);
            break;
        case T_ARR:
            destroy_array(val.arrayv);
            break;
        case T_DICT:
            destroy_dict(val.dictv);
            break;
        }
    }
    free(values);
#else
    ValueLink *link = a->head;
    while (link)
    {
        ValueLink *tmp = link;
        Value *values = link->values;
        link = link->next;
        for (unsigned i = 0; i < ARRAY_LEN; ++i)
        {
            switch (values[i].type)
            {
            case T_STR:
                free(values[i].strv.str);
                break;
            case T_ARR:
                destroy_array(values[i].arrayv);
                break;
            case T_DICT:
                destroy_dict(values[i].dictv);
                break;
            }
        }
        free(tmp);
    }
#endif // !EDITING_MODE
    free(a);
}

void destroy_dict(Dict *d)
{
    if (!d)
    {
        return;
    }

#ifndef EDITING_MODE
    Item *items = d->items;
    unsigned size = d->size;
    for (unsigned i = 0; i < size; ++i)
    {
        Item it = items[i];
        free(it.key.str);
        switch (it.type)
        {
        case T_STR:
            free(it.strv.str);
            break;
        case T_ARR:
            destroy_array(it.arrayv);
            break;
        case T_DICT:
            destroy_dict(it.dictv);
            break;
        }
    }
    free(items);
#else
    ItemLink *link = d->head;
    while (link)
    {
        ItemLink *tmp = link;
        Item *items = link->items;
        link = link->next;
        for (unsigned i = 0; i < ARRAY_LEN; ++i)
        {
            switch (items[i].type)
            {
            case T_STR:
                free(items[i].strv.str);
                break;
            case T_ARR:
                destroy_array(items[i].arrayv);
                break;
            case T_DICT:
                destroy_dict(items[i].dictv);
                break;
            }
            free(items[i].key.str);
        }
        free(tmp);
    }
#endif // !EDITING_MODE
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
