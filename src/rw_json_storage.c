#include "rw_json_storage.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdlib.h>

/*******************************************************************************
**                                   MACROS                                   **
*******************************************************************************/
/**
** \def Adds an element to the given array_t or dict_t
** \param TypeLink Can be either 'ValueLink' or 'ItemLink'
** \param x The name of the array_t or dict_t variable (a for array_t, d for
**          dict_t)
*/
#define ADD(TypeLink, x)                                                       \
    if (!x->head)                                                              \
    {                                                                          \
        TypeLink *l = calloc(1, sizeof(TypeLink));                             \
        if (!l)                                                                \
        {                                                                      \
            return;                                                            \
        }                                                                      \
        x->head = l;                                                           \
        x->tail = l;                                                           \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        if (!x->tail)                                                          \
        {                                                                      \
            return;                                                            \
        }                                                                      \
        /* If the current link's array_t is full */                            \
        if (x->tail->insert_index >= ARRAY_LEN)                                \
        {                                                                      \
            TypeLink *l = calloc(1, sizeof(TypeLink));                         \
            if (!l)                                                            \
            {                                                                  \
                return;                                                        \
            }                                                                  \
            x->tail->next = l;                                                 \
            x->tail = l;                                                       \
        }                                                                      \
    }                                                                          \
    ++x->size

/*******************************************************************************
**                               LOCAL FUNCTIONS                              **
*******************************************************************************/
#ifdef DEBUG
#    include <stdio.h>
/**
** \brief Debug function that allows printing the linked lists arrays
*/
void arr_print_array(array_t *a)
{
    if (!a)
    {
        return;
    }

    value_link_t *link = a->head;
    while (link)
    {
        printf("[");
        value_t *values = link->values;
        for (unsigned i = 0; i < ARRAY_LEN; ++i)
        {
            if (values[i].type != 0)
            {
                value_t v = values[i];
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
                    printf("array_t, ");
                    break;
                case T_DICT:
                    printf("dict_t, ");
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
#endif // !DEBUG

void arr_copy_array(value_t *src, value_t *dest)
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

void dict_copy_array(item_t *src, item_t *dest)
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

void arr_empty_array(value_t *array_t)
{
    if (!array_t)
    {
        return;
    }

    for (unsigned i = 0; i < ARRAY_LEN; ++i)
    {
        array_t[i].type = 0;
    }
}

void dict_empty_array(item_t *array_t)
{
    if (!array_t)
    {
        return;
    }

    for (unsigned i = 0; i < ARRAY_LEN; ++i)
    {
        array_t[i].type = 0;
    }
}

void defragment_array(array_t *a)
{
    if (!a)
    {
        return;
    }

    // We fill this array_t with each non-empty element (where the type is not
    // T_ERROR), and then we copy its contents to the previously fragmented
    // array_t
    value_t tmps[ARRAY_LEN] = { 0 };
    unsigned tmps_insert_idx = 0;

    value_link_t *link_to_fill = a->head;
    value_link_t *prev_link_to_fill = a->head;
    char last_started_fill = 0;

    value_link_t *link = a->head;
    // We gather all the non empty values inside 'tmps', and then we copy the
    // contents of 'tmps' in the 'link_to_fill' (which starts at the head)
    while (link)
    {
        for (unsigned i = 0; i < ARRAY_LEN; ++i)
        {
            // The temp array_t is filled, so we can put the defragmented
            // array_t back inside it
            if (tmps_insert_idx == ARRAY_LEN)
            {
                // Copies the temp array_t to the base array_t
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

    // When the last link's array_t didn't reach the max number of contained
    // elements, we add the remaining elements and free all the remaining unused
    // links
    if (tmps_insert_idx)
    {
        arr_copy_array(tmps, link_to_fill->values);
        a->tail = link_to_fill;
        link_to_fill = link_to_fill->next;
        while (link_to_fill)
        {
            value_link_t *tmp = link_to_fill;
            link_to_fill = link_to_fill->next;
            free(tmp);
        }
        a->tail->next = 0;
    }
    // If we filled the array_t in the linked list but there is no value_t left
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
            value_link_t *tmp = link_to_fill;
            link_to_fill = link_to_fill->next;
            free(tmp);
        }
    }

    // Sets the insert index of the last link to the correct position
    value_t *values = a->tail->values;
    for (unsigned i = 0; i < ARRAY_LEN; ++i)
    {
        if (values[i].type == T_ERROR)
        {
            a->tail->insert_index = i;
            break;
        }
    }
}

void defragment_dict(dict_t *d)
{
    if (!d)
    {
        return;
    }

    // We fill this array_t with each non-empty element (where the type is not
    // T_ERROR), and then we copy its contents to the previously fragmented
    // array_t
    item_t tmps[ARRAY_LEN] = { 0 };
    unsigned tmps_insert_idx = 0;

    item_link_t *link_to_fill = d->head;
    item_link_t *prev_link_to_fill = d->head;
    char last_started_fill = 0;

    item_link_t *link = d->head;
    // We gather all the non empty items inside 'tmps', and then we copy the
    // contents of 'tmps' in the 'link_to_fill' (which starts at the head)
    while (link)
    {
        for (unsigned i = 0; i < ARRAY_LEN; ++i)
        {
            // The temp array_t is filled, so we can put the defragmented
            // array_t back inside it
            if (tmps_insert_idx == ARRAY_LEN)
            {
                // Copies the temp array_t to the base dict_t
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

    // When the last link's array_t didn't reach the max number of contained
    // elements, we add the remaining elements and free all the remaining unused
    // links
    if (tmps_insert_idx)
    {
        dict_copy_array(tmps, link_to_fill->items);
        d->tail = link_to_fill;
        link_to_fill = link_to_fill->next;
        while (link_to_fill)
        {
            item_link_t *tmp = link_to_fill;
            link_to_fill = link_to_fill->next;
            free(tmp);
        }
        d->tail->next = 0;
    }
    // If we filled the array_t in the linked list but there is no value_t left
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
            item_link_t *tmp = link_to_fill;
            link_to_fill = link_to_fill->next;
            free(tmp);
        }
    }

    // Sets the insert index of the last link to the correct position
    item_t *items = d->tail->items;
    for (unsigned i = 0; i < ARRAY_LEN; ++i)
    {
        if (items[i].type == T_ERROR)
        {
            d->tail->insert_index = i;
            break;
        }
    }
}

/*******************************************************************************
**                                     ADDS                                   **
*******************************************************************************/
void arr_add_str(array_t *a, string_t value)
{
    if (a)
    {
        ADD(value_link_t, a);
        a->tail->values[a->tail->insert_index++] =
            (value_t){ .type = T_STR, .strv = value };
    }
}

void arr_add_int(array_t *a, int value)
{
    if (a)
    {
        ADD(value_link_t, a);
        a->tail->values[a->tail->insert_index++] =
            (value_t){ .type = T_INT, .intv = value };
    }
}

void arr_add_double(array_t *a, double value)
{
    if (a)
    {
        ADD(value_link_t, a);
        a->tail->values[a->tail->insert_index++] =
            (value_t){ .type = T_DOUBLE, .doublev = value };
    }
}

void arr_add_bool(array_t *a, char value)
{
    if (a)
    {
        ADD(value_link_t, a);
        a->tail->values[a->tail->insert_index++] =
            (value_t){ .type = T_BOOL, .boolv = value };
    }
}

void arr_add_null(array_t *a)
{
    if (a)
    {
        ADD(value_link_t, a);
        a->tail->values[a->tail->insert_index++] = (value_t){ .type = T_NULL };
    }
}

void arr_add_arr(array_t *a, array_t *value)
{
    if (a && value)
    {
        ADD(value_link_t, a);
        a->tail->values[a->tail->insert_index++] =
            (value_t){ .type = T_ARR, .arrayv = value };
    }
}

void arr_add_dict(array_t *a, dict_t *value)
{
    if (a && value)
    {
        ADD(value_link_t, a);
        a->tail->values[a->tail->insert_index++] =
            (value_t){ .type = T_DICT, .dictv = value };
    }
}

void dict_add_str(dict_t *d, string_t key, string_t value_t)
{
    if (d)
    {
        ADD(item_link_t, d);
        d->tail->items[d->tail->insert_index++] =
            (item_t){ .type = T_STR, .key = key, .strv = value_t };
    }
}

void dict_add_int(dict_t *d, string_t key, int value_t)
{
    if (d)
    {
        ADD(item_link_t, d);
        d->tail->items[d->tail->insert_index++] =
            (item_t){ .type = T_INT, .key = key, .intv = value_t };
    }
}

void dict_add_double(dict_t *d, string_t key, double value_t)
{
    if (d)
    {
        ADD(item_link_t, d);
        d->tail->items[d->tail->insert_index++] =
            (item_t){ .type = T_DOUBLE, .key = key, .doublev = value_t };
    }
}

void dict_add_bool(dict_t *d, string_t key, char value_t)
{
    if (d)
    {
        ADD(item_link_t, d);
        d->tail->items[d->tail->insert_index++] =
            (item_t){ .type = T_BOOL, .key = key, .boolv = value_t };
    }
}

void dict_add_null(dict_t *d, string_t key)
{
    if (d)
    {
        ADD(item_link_t, d);
        d->tail->items[d->tail->insert_index++] =
            (item_t){ .type = T_NULL, .key = key };
    }
}

void dict_add_arr(dict_t *d, string_t key, array_t *value_t)
{
    if (d && value_t)
    {
        ADD(item_link_t, d);
        d->tail->items[d->tail->insert_index++] =
            (item_t){ .type = T_ARR, .key = key, .arrayv = value_t };
    }
}

void dict_add_dict(dict_t *d, string_t key, dict_t *value_t)
{
    if (d && value_t)
    {
        ADD(item_link_t, d);
        d->tail->items[d->tail->insert_index++] =
            (item_t){ .type = T_DICT, .key = key, .dictv = value_t };
    }
}

/*******************************************************************************
**                                   REMOVES                                  **
*******************************************************************************/
void arr_remove(array_t *a, unsigned index)
{
    if (!a || index >= a->size)
    {
        return;
    }

    value_link_t *link = a->head;
    unsigned non_null_values = 0;
    while (link)
    {
        for (unsigned i = 0; i < ARRAY_LEN; ++i)
        {
            value_t v = link->values[i];
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

void dict_remove(dict_t *d, string_t key)
{
    if (!d || !key.str)
    {
        return;
    }

    item_link_t *link = d->head;
    while (link)
    {
        for (unsigned i = 0; i < ARRAY_LEN; ++i)
        {
            item_t it = link->items[i];
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

/*******************************************************************************
**                                    GETS                                    **
*******************************************************************************/
value_t array_get(array_t *a, unsigned index)
{
    if (!a || index >= a->size)
    {
        return ERROR_VALUE;
    }

    value_link_t *link = a->head;
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
}

item_t dict_get(dict_t *d, string_t key)
{
    if (!d)
    {
        return ERROR_ITEM;
    }

    item_link_t *link = d->head;
    unsigned non_null_values = 0;
    while (link)
    {
        for (unsigned i = 0; i < ARRAY_LEN; ++i)
        {
            item_t it = link->items[i];
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
    return ERROR_ITEM;
}

/*******************************************************************************
**                                   DESTROYS                                 **
*******************************************************************************/
void destroy_array(array_t *a)
{
    if (!a)
    {
        return;
    }

    value_link_t *link = a->head;
    while (link)
    {
        value_link_t *tmp = link;
        value_t *values = link->values;
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
    free(a);
}

void destroy_dict(dict_t *d)
{
    if (!d)
    {
        return;
    }

    item_link_t *link = d->head;
    while (link)
    {
        item_link_t *tmp = link;
        item_t *items = link->items;
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
    free(d);
}

void destroy_json(json_t *j)
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
