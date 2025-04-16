#include "rw_json_storage.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base_json_storage.h"

/*******************************************************************************
**                                   MACROS                                   **
*******************************************************************************/
/**
** \def Adds an element to the given rw_array_t or rw_dict_t
** \param TypeLink Can be either 'ValueLink' or 'ItemLink'
** \param x The name of the rw_array_t or rw_dict_t variable (a for array_t, d
**          for rw_dict_t)
*/
#define ADD(type_link, x)                                                      \
    if (!x->head)                                                              \
    {                                                                          \
        type_link *l = calloc(1, sizeof(type_link));                           \
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
        /* If the current link's rw_array_t is full */                         \
        if (x->tail->insert_index >= ARRAY_LEN)                                \
        {                                                                      \
            type_link *l = calloc(1, sizeof(type_link));                       \
            if (!l)                                                            \
            {                                                                  \
                return;                                                        \
            }                                                                  \
            x->tail->next = l;                                                 \
            x->tail       = l;                                                 \
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
void arr_print_array(rw_array_t *a)
{
    if (!a)
    {
        return;
    }

    value_link_t *link = a->head;
    while (link)
    {
        printf("[");
        rw_value_t *values = link->values;
        for (arr_size_t i = 0; i < ARRAY_LEN; ++i)
        {
            if (values[i].type != 0)
            {
                rw_value_t v = values[i];
                switch (v.type)
                {
                case T_STR:
                    printf("\"%s\", ", v.strv.str ? v.strv.str : "");
                    break;
                case T_LONG:
                    printf("%ld, ", v.longv);
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
                    printf("rw_array_t, ");
                    break;
                case T_DICT:
                    printf("rw_dict_t, ");
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

void arr_copy_array(rw_value_t *src, rw_value_t *dest)
{
    if (!src || !dest)
    {
        return;
    }

    for (arr_size_t i = 0; i < ARRAY_LEN; ++i)
    {
        dest[i] = src[i];
    }
}

void dict_copy_array(rw_item_t *src, rw_item_t *dest)
{
    if (!src || !dest)
    {
        return;
    }

    for (arr_size_t i = 0; i < ARRAY_LEN; ++i)
    {
        dest[i] = src[i];
    }
}

void arr_empty_array(rw_value_t *rw_array_t)
{
    if (!rw_array_t)
    {
        return;
    }

    for (arr_size_t i = 0; i < ARRAY_LEN; ++i)
    {
        rw_array_t[i].type = 0;
    }
}

void dict_empty_array(rw_item_t *rw_array_t)
{
    if (!rw_array_t)
    {
        return;
    }

    for (arr_size_t i = 0; i < ARRAY_LEN; ++i)
    {
        rw_array_t[i].type = 0;
    }
}

void defragment_array(rw_array_t *a)
{
    if (!a)
    {
        return;
    }

    // We fill this rw_array_t with each non-empty element (where the type is
    // not T_ERROR), and then we copy its contents to the previously fragmented
    // rw_array_t
    rw_value_t tmps[ARRAY_LEN] = { 0 };
    arr_size_t tmps_insert_idx = 0;

    value_link_t *link_to_fill      = a->head;
    value_link_t *prev_link_to_fill = a->head;
    bool last_started_fill          = false;

    value_link_t *link = a->head;
    // We gather all the non empty values inside 'tmps', and then we copy the
    // contents of 'tmps' in the 'link_to_fill' (which starts at the head)
    while (link)
    {
        for (arr_size_t i = 0; i < ARRAY_LEN; ++i)
        {
            // The temp rw_array_t is filled, so we can put the defragmented
            // rw_array_t back inside it
            if (tmps_insert_idx == ARRAY_LEN)
            {
                // Copies the temp rw_array_t to the base array_t
                arr_copy_array(tmps, link_to_fill->values);
                link_to_fill->insert_index = ARRAY_LEN;
                arr_empty_array(tmps);
                tmps_insert_idx   = 0;
                prev_link_to_fill = link_to_fill;
                link_to_fill      = link_to_fill->next;
                last_started_fill = false;
            }

            if (link->values[i].type != T_ERROR)
            {
                tmps[tmps_insert_idx++] = link->values[i];
                last_started_fill       = true;
            }
        }
        link = link->next;
    }

    // When the last link's rw_array_t didn't reach the max number of contained
    // elements, we add the remaining elements and free all the remaining unused
    // links
    if (tmps_insert_idx)
    {
        arr_copy_array(tmps, link_to_fill->values);
        a->tail      = link_to_fill;
        link_to_fill = link_to_fill->next;
        while (link_to_fill)
        {
            value_link_t *tmp = link_to_fill;
            link_to_fill      = link_to_fill->next;
            free(tmp);
        }
        a->tail->next = 0;
    }
    // If we filled the rw_array_t in the linked list but there is no rw_value_t
    // left after, it means that the 'link_to_fill' now points to a link that we
    // have to remove, because the defragmentation is done so it is no longer
    // the link to fill. This is why we make the tail point to the previous one,
    // and free all the remaining links starting at 'link_to_fill'
    else if (!last_started_fill)
    {
        a->tail       = prev_link_to_fill;
        a->tail->next = 0;
        while (link_to_fill)
        {
            value_link_t *tmp = link_to_fill;
            link_to_fill      = link_to_fill->next;
            free(tmp);
        }
    }

    // Sets the insert index of the last link to the correct position
    rw_value_t *values = a->tail->values;
    for (arr_size_t i = 0; i < ARRAY_LEN; ++i)
    {
        if (values[i].type == T_ERROR)
        {
            a->tail->insert_index = i;
            break;
        }
    }
}

void defragment_dict(rw_dict_t *d)
{
    if (!d)
    {
        return;
    }

    // We fill this rw_array_t with each non-empty element (where the type is
    // not T_ERROR), and then we copy its contents to the previously fragmented
    // rw_array_t
    rw_item_t tmps[ARRAY_LEN]  = { 0 };
    arr_size_t tmps_insert_idx = 0;

    item_link_t *link_to_fill      = d->head;
    item_link_t *prev_link_to_fill = d->head;
    bool last_started_fill         = false;

    item_link_t *link = d->head;
    // We gather all the non empty items inside 'tmps', and then we copy the
    // contents of 'tmps' in the 'link_to_fill' (which starts at the head)
    while (link)
    {
        for (arr_size_t i = 0; i < ARRAY_LEN; ++i)
        {
            // The temp rw_array_t is filled, so we can put the defragmented
            // rw_array_t back inside it
            if (tmps_insert_idx == ARRAY_LEN)
            {
                // Copies the temp rw_array_t to the base rw_dict_t
                dict_copy_array(tmps, link_to_fill->items);
                link_to_fill->insert_index = ARRAY_LEN;
                dict_empty_array(tmps);
                tmps_insert_idx   = 0;
                prev_link_to_fill = link_to_fill;
                link_to_fill      = link_to_fill->next;
                last_started_fill = 0;
            }

            if (link->items[i].type != T_ERROR)
            {
                tmps[tmps_insert_idx++] = link->items[i];
                last_started_fill       = 1;
            }
        }
        link = link->next;
    }

    // When the last link's rw_array_t didn't reach the max number of contained
    // elements, we add the remaining elements and free all the remaining unused
    // links
    if (tmps_insert_idx)
    {
        dict_copy_array(tmps, link_to_fill->items);
        d->tail      = link_to_fill;
        link_to_fill = link_to_fill->next;
        while (link_to_fill)
        {
            item_link_t *tmp = link_to_fill;
            link_to_fill     = link_to_fill->next;
            free(tmp);
        }
        d->tail->next = 0;
    }
    // If we filled the rw_array_t in the linked list but there is no rw_value_t
    // left after, it means that the 'link_to_fill' now points to a link that we
    // have to remove, because the defragmentation is done so it is no longer
    // the link to fill. This is why we make the tail point to the previous one,
    // and free all the remaining links starting at 'link_to_fill'
    else if (!last_started_fill)
    {
        d->tail       = prev_link_to_fill;
        d->tail->next = 0;
        while (link_to_fill)
        {
            item_link_t *tmp = link_to_fill;
            link_to_fill     = link_to_fill->next;
            free(tmp);
        }
    }

    // Sets the insert index of the last link to the correct position
    rw_item_t *items = d->tail->items;
    for (arr_size_t i = 0; i < ARRAY_LEN; ++i)
    {
        if (items[i].type == T_ERROR)
        {
            d->tail->insert_index = i;
            break;
        }
    }
}

rw_array_t init_rw_array_with(size_t size, ...)
{
    if (!size)
    {
        return EMPTY_RW_ARRAY;
    }

    rw_array_t a = EMPTY_RW_ARRAY;

    va_list args;

    va_start(args, size);
    for (size_t i = 0; i < size; ++i)
    {
        rw_value_t v = va_arg(args, rw_value_t);
        switch (v.type)
        {
        case T_ERROR:
            break;
        case T_STR:
            rw_array_add_str(&a, v.strv);
            break;
        case T_LONG:
            rw_array_add_long(&a, v.longv);
            break;
        case T_DOUBLE:
            rw_array_add_double(&a, v.doublev);
            break;
        case T_EXP_LONG:
            rw_array_add_exp_long(&a, v.exp_longv);
            break;
        case T_EXP_DOUBLE:
            rw_array_add_exp_double(&a, v.exp_doublev);
            break;
        case T_BOOL:
            rw_array_add_bool(&a, v.boolv);
            break;
        case T_NULL:
            rw_array_add_null(&a);
            break;
        case T_ARR:
            rw_array_add_array(&a, v.arrayv);
            break;
        case T_DICT:
            rw_array_add_dict(&a, v.dictv);
            break;
        }
    }

    va_end(args);
    return a;
}

rw_dict_t init_rw_dict_with(size_t size, ...)
{
    if (!size)
    {
        return EMPTY_RW_DICT;
    }

    rw_dict_t d = EMPTY_RW_DICT;

    va_list args;

    va_start(args, size);
    for (size_t i = 0; i < size; ++i)
    {
        rw_item_t it = va_arg(args, rw_item_t);
        switch (it.type)
        {
        case T_ERROR:
            break;
        case T_STR:
            rw_dict_add_str(&d, it.key, it.strv);
            break;
        case T_LONG:
            rw_dict_add_long(&d, it.key, it.longv);
            break;
        case T_DOUBLE:
            rw_dict_add_double(&d, it.key, it.doublev);
            break;
        case T_EXP_LONG:
            rw_dict_add_exp_long(&d, it.key, it.exp_longv);
            break;
        case T_EXP_DOUBLE:
            rw_dict_add_exp_double(&d, it.key, it.exp_doublev);
            break;
        case T_BOOL:
            rw_dict_add_bool(&d, it.key, it.boolv);
            break;
        case T_NULL:
            rw_dict_add_null(&d, it.key);
            break;
        case T_ARR:
            rw_dict_add_array(&d, it.key, it.arrayv);
            break;
        case T_DICT:
            rw_dict_add_dict(&d, it.key, it.dictv);
            break;
        }
    }

    va_end(args);
    return d;
}

/*******************************************************************************
**                                     ADDS                                   **
*******************************************************************************/
void rw_array_add_str(rw_array_t *a, string_t value)
{
    if (a && value.str)
    {
        ADD(value_link_t, a);
        a->tail->values[a->tail->insert_index++] = RW_VALUE_OF(T_STR, strv);
    }
}

void rw_array_add_long(rw_array_t *a, i64 value)
{
    if (a)
    {
        ADD(value_link_t, a);
        a->tail->values[a->tail->insert_index++] = RW_VALUE_OF(T_LONG, longv);
    }
}

void rw_array_add_double(rw_array_t *a, double value)
{
    if (a)
    {
        ADD(value_link_t, a);
        a->tail->values[a->tail->insert_index++] =
            RW_VALUE_OF(T_DOUBLE, doublev);
    }
}

void rw_array_add_exp_long(rw_array_t *a, exp_long_t value)
{
    if (a)
    {
        ADD(value_link_t, a);
        a->tail->values[a->tail->insert_index++] =
            RW_VALUE_OF(T_EXP_LONG, exp_longv);
    }
}

void rw_array_add_exp_double(rw_array_t *a, exp_double_t value)
{
    if (a)
    {
        ADD(value_link_t, a);
        a->tail->values[a->tail->insert_index++] =
            RW_VALUE_OF(T_EXP_DOUBLE, exp_doublev);
    }
}

void rw_array_add_bool(rw_array_t *a, bool value)
{
    if (a)
    {
        ADD(value_link_t, a);
        a->tail->values[a->tail->insert_index++] = RW_VALUE_OF(T_BOOL, boolv);
    }
}

void rw_array_add_null(rw_array_t *a)
{
    if (a)
    {
        ADD(value_link_t, a);
        a->tail->values[a->tail->insert_index++] =
            (rw_value_t){ .type = T_NULL };
    }
}

void rw_array_add_array(rw_array_t *a, rw_array_t value)
{
    if (a)
    {
        ADD(value_link_t, a);
        a->tail->values[a->tail->insert_index++] = RW_VALUE_OF(T_ARR, arrayv);
    }
}

void rw_array_add_dict(rw_array_t *a, rw_dict_t value)
{
    if (a)
    {
        ADD(value_link_t, a);
        a->tail->values[a->tail->insert_index++] = RW_VALUE_OF(T_DICT, dictv);
    }
}

void rw_dict_add_str(rw_dict_t *d, string_t key, string_t value)
{
    if (d && key.str)
    {
        ADD(item_link_t, d);
        d->tail->items[d->tail->insert_index++] = RW_ITEM_OF(T_STR, strv);
    }
}

void rw_dict_add_long(rw_dict_t *d, string_t key, i64 value)
{
    if (d && key.str)
    {
        ADD(item_link_t, d);
        d->tail->items[d->tail->insert_index++] = RW_ITEM_OF(T_LONG, longv);
    }
}

void rw_dict_add_double(rw_dict_t *d, string_t key, double value)
{
    if (d && key.str)
    {
        ADD(item_link_t, d);
        d->tail->items[d->tail->insert_index++] = RW_ITEM_OF(T_DOUBLE, doublev);
    }
}

void rw_dict_add_exp_long(rw_dict_t *d, string_t key, exp_long_t value)
{
    if (d && key.str)
    {
        ADD(item_link_t, d);
        d->tail->items[d->tail->insert_index++] =
            RW_ITEM_OF(T_EXP_LONG, exp_longv);
    }
}

void rw_dict_add_exp_double(rw_dict_t *d, string_t key, exp_double_t value)
{
    if (d && key.str)
    {
        ADD(item_link_t, d);
        d->tail->items[d->tail->insert_index++] =
            RW_ITEM_OF(T_EXP_DOUBLE, exp_doublev);
    }
}

void rw_dict_add_bool(rw_dict_t *d, string_t key, bool value)
{
    if (d && key.str)
    {
        ADD(item_link_t, d);
        d->tail->items[d->tail->insert_index++] = RW_ITEM_OF(T_BOOL, boolv);
    }
}

void rw_dict_add_null(rw_dict_t *d, string_t key)
{
    if (d && key.str)
    {
        ADD(item_link_t, d);
        d->tail->items[d->tail->insert_index++] =
            (rw_item_t){ .type = T_NULL, .key = key };
    }
}

void rw_dict_add_array(rw_dict_t *d, string_t key, rw_array_t value)
{
    if (d && key.str)
    {
        ADD(item_link_t, d);
        d->tail->items[d->tail->insert_index++] = RW_ITEM_OF(T_ARR, arrayv);
    }
}

void rw_dict_add_dict(rw_dict_t *d, string_t key, rw_dict_t value)
{
    if (d && key.str)
    {
        ADD(item_link_t, d);
        d->tail->items[d->tail->insert_index++] = RW_ITEM_OF(T_DICT, dictv);
    }
}

/*******************************************************************************
**                                   REMOVES                                  **
*******************************************************************************/
void rw_array_remove(rw_array_t *a, size_t index)
{
    if (!a || index >= a->size)
    {
        return;
    }

    value_link_t *link     = a->head;
    size_t non_null_values = 0;
    while (link)
    {
        for (arr_size_t i = 0; i < ARRAY_LEN; ++i)
        {
            rw_value_t v = link->values[i];
            if (v.type != T_ERROR)
            {
                ++non_null_values;
            }

            if (non_null_values && non_null_values - 1 == index)
            {
                switch (v.type)
                {
                case T_STR:
                    destroy_string(v.strv);
                    v.strv.str = 0;
                    break;
                case T_ARR:
                    destroy_rw_array(v.arrayv);
                    v.arrayv = EMPTY_RW_ARRAY;
                    break;
                case T_DICT:
                    destroy_rw_dict(v.dictv);
                    v.dictv = EMPTY_RW_DICT;
                    break;
                default:
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

void rw_dict_remove(rw_dict_t *d, string_t key)
{
    if (!d || !key.str)
    {
        return;
    }

    item_link_t *link = d->head;
    while (link)
    {
        for (arr_size_t i = 0; i < ARRAY_LEN; ++i)
        {
            rw_item_t it = link->items[i];
            if (it.type != T_ERROR && strings_equals(key, it.key))
            {
                switch (it.type)
                {
                case T_STR:
                    destroy_string(it.strv);
                    it.strv.str = 0;
                    break;
                case T_ARR:
                    destroy_rw_array(it.arrayv);
                    it.arrayv = EMPTY_RW_ARRAY;
                    break;
                case T_DICT:
                    destroy_rw_dict(it.dictv);
                    it.dictv = EMPTY_RW_DICT;
                    break;
                default:
                    break;
                };
                free(it.key.str);
                it.key.str          = 0;
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
rw_value_t rw_array_get(rw_array_t a, size_t index)
{
    if (index >= a.size)
    {
        return ERROR_RW_VALUE;
    }

    value_link_t *link     = a.head;
    size_t non_null_values = 0;
    while (link)
    {
        for (arr_size_t i = 0; i < ARRAY_LEN; ++i)
        {
            if (non_null_values == index)
            {
                return link->values[i];
            }
            non_null_values += link->values[i].type != T_ERROR;
        }
        link = link->next;
    }
    return ERROR_RW_VALUE;
}

rw_item_t rw_dict_get(rw_dict_t d, string_t key)
{
    item_link_t *link = d.head;
    while (link)
    {
        for (arr_size_t i = 0; i < ARRAY_LEN; ++i)
        {
            rw_item_t it = link->items[i];
            if (strings_equals(key, it.key))
            {
                return it;
            }
        }
        link = link->next;
    }
    return ERROR_RW_ITEM;
}

/*******************************************************************************
**                                 DESTRUCTION                                **
*******************************************************************************/
void destroy_rw_array(rw_array_t a)
{
    value_link_t *link = a.head;
    while (link)
    {
        value_link_t *tmp  = link;
        rw_value_t *values = link->values;
        link               = link->next;
        for (arr_size_t i = 0; i < ARRAY_LEN; ++i)
        {
            switch (values[i].type)
            {
            case T_STR:
                destroy_string(values[i].strv);
                break;
            case T_ARR:
                destroy_rw_array(values[i].arrayv);
                break;
            case T_DICT:
                destroy_rw_dict(values[i].dictv);
                break;
            default:
                break;
            }
        }
        free(tmp);
    }
}

void destroy_rw_dict(rw_dict_t d)
{
    item_link_t *link = d.head;
    while (link)
    {
        item_link_t *tmp = link;
        rw_item_t *items = link->items;
        link             = link->next;
        for (arr_size_t i = 0; i < ARRAY_LEN; ++i)
        {
            switch (items[i].type)
            {
            case T_STR:
                destroy_string(items[i].strv);
                break;
            case T_ARR:
                destroy_rw_array(items[i].arrayv);
                break;
            case T_DICT:
                destroy_rw_dict(items[i].dictv);
                break;
            default:
                break;
            }
            if (items[i].type != T_ERROR)
            {
                destroy_string(items[i].key);
            }
        }
        free(tmp);
    }
}

void destroy_rw_json(rw_json_t j)
{
    if (j.is_array)
    {
        destroy_rw_array(j.array);
    }
    else
    {
        destroy_rw_dict(j.dict);
    }
}
