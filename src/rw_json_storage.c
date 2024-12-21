#include "rw_json_storage.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
**                                   MACROS                                   **
*******************************************************************************/
/**
** \def Adds an element to the given rw_array_t or rw_dict_t
** \param TypeLink Can be either 'ValueLink' or 'ItemLink'
** \param x The name of the rw_array_t or rw_dict_t variable (a for array_t, d
*for
**          rw_dict_t)
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
        /* If the current link's rw_array_t is full */                         \
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
        for (unsigned i = 0; i < ARRAY_LEN; ++i)
        {
            if (values[i].type != 0)
            {
                rw_value_t v = values[i];
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

    for (unsigned i = 0; i < ARRAY_LEN; ++i)
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

    for (unsigned i = 0; i < ARRAY_LEN; ++i)
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

    for (unsigned i = 0; i < ARRAY_LEN; ++i)
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

    for (unsigned i = 0; i < ARRAY_LEN; ++i)
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
            // The temp rw_array_t is filled, so we can put the defragmented
            // rw_array_t back inside it
            if (tmps_insert_idx == ARRAY_LEN)
            {
                // Copies the temp rw_array_t to the base array_t
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

    // When the last link's rw_array_t didn't reach the max number of contained
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
    // If we filled the rw_array_t in the linked list but there is no rw_value_t
    // left after, it means that the 'link_to_fill' now points to a link that we
    // have to remove, because the defragmentation is done so it is no longer
    // the link to fill. This is why we make the tail point to the previous one,
    // and free all the remaining links starting at 'link_to_fill'
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
    rw_value_t *values = a->tail->values;
    for (unsigned i = 0; i < ARRAY_LEN; ++i)
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
    rw_item_t tmps[ARRAY_LEN] = { 0 };
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
            // The temp rw_array_t is filled, so we can put the defragmented
            // rw_array_t back inside it
            if (tmps_insert_idx == ARRAY_LEN)
            {
                // Copies the temp rw_array_t to the base rw_dict_t
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

    // When the last link's rw_array_t didn't reach the max number of contained
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
    // If we filled the rw_array_t in the linked list but there is no rw_value_t
    // left after, it means that the 'link_to_fill' now points to a link that we
    // have to remove, because the defragmentation is done so it is no longer
    // the link to fill. This is why we make the tail point to the previous one,
    // and free all the remaining links starting at 'link_to_fill'
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
    rw_item_t *items = d->tail->items;
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
**                                 FUNCTIONS                                  **
*******************************************************************************/
rw_json_t *init_ro_json(char is_array, rw_array_t *a, rw_dict_t *d)
{
    rw_json_t *j = calloc(1, sizeof(rw_json_t));
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
void arr_add_str(rw_array_t *a, string_t value)
{
    if (a)
    {
        ADD(value_link_t, a);
        a->tail->values[a->tail->insert_index++] =
            (rw_value_t){ .type = T_STR, .strv = value };
    }
}

void arr_add_int(rw_array_t *a, int value)
{
    if (a)
    {
        ADD(value_link_t, a);
        a->tail->values[a->tail->insert_index++] =
            (rw_value_t){ .type = T_INT, .intv = value };
    }
}

void arr_add_double(rw_array_t *a, double value)
{
    if (a)
    {
        ADD(value_link_t, a);
        a->tail->values[a->tail->insert_index++] =
            (rw_value_t){ .type = T_DOUBLE, .doublev = value };
    }
}

void arr_add_bool(rw_array_t *a, char value)
{
    if (a)
    {
        ADD(value_link_t, a);
        a->tail->values[a->tail->insert_index++] =
            (rw_value_t){ .type = T_BOOL, .boolv = value };
    }
}

void arr_add_null(rw_array_t *a)
{
    if (a)
    {
        ADD(value_link_t, a);
        a->tail->values[a->tail->insert_index++] =
            (rw_value_t){ .type = T_NULL };
    }
}

void arr_add_arr(rw_array_t *a, rw_array_t *value)
{
    if (a && value)
    {
        ADD(value_link_t, a);
        a->tail->values[a->tail->insert_index++] =
            (rw_value_t){ .type = T_ARR, .arrayv = value };
    }
}

void arr_add_dict(rw_array_t *a, rw_dict_t *value)
{
    if (a && value)
    {
        ADD(value_link_t, a);
        a->tail->values[a->tail->insert_index++] =
            (rw_value_t){ .type = T_DICT, .dictv = value };
    }
}

void dict_add_str(rw_dict_t *d, string_t key, string_t rw_value_t)
{
    if (d)
    {
        ADD(item_link_t, d);
        d->tail->items[d->tail->insert_index++] =
            (rw_item_t){ .type = T_STR, .key = key, .strv = rw_value_t };
    }
}

void dict_add_int(rw_dict_t *d, string_t key, int rw_value_t)
{
    if (d)
    {
        ADD(item_link_t, d);
        d->tail->items[d->tail->insert_index++] =
            (rw_item_t){ .type = T_INT, .key = key, .intv = rw_value_t };
    }
}

void dict_add_double(rw_dict_t *d, string_t key, double rw_value_t)
{
    if (d)
    {
        ADD(item_link_t, d);
        d->tail->items[d->tail->insert_index++] =
            (rw_item_t){ .type = T_DOUBLE, .key = key, .doublev = rw_value_t };
    }
}

void dict_add_bool(rw_dict_t *d, string_t key, char rw_value_t)
{
    if (d)
    {
        ADD(item_link_t, d);
        d->tail->items[d->tail->insert_index++] =
            (rw_item_t){ .type = T_BOOL, .key = key, .boolv = rw_value_t };
    }
}

void dict_add_null(rw_dict_t *d, string_t key)
{
    if (d)
    {
        ADD(item_link_t, d);
        d->tail->items[d->tail->insert_index++] =
            (rw_item_t){ .type = T_NULL, .key = key };
    }
}

void dict_add_arr(rw_dict_t *d, string_t key, rw_array_t *rw_value_t)
{
    if (d && rw_value_t)
    {
        ADD(item_link_t, d);
        d->tail->items[d->tail->insert_index++] =
            (rw_item_t){ .type = T_ARR, .key = key, .arrayv = rw_value_t };
    }
}

void dict_add_dict(rw_dict_t *d, string_t key, rw_dict_t *rw_value_t)
{
    if (d && rw_value_t)
    {
        ADD(item_link_t, d);
        d->tail->items[d->tail->insert_index++] =
            (rw_item_t){ .type = T_DICT, .key = key, .dictv = rw_value_t };
    }
}

/*******************************************************************************
**                                   REMOVES                                  **
*******************************************************************************/
void arr_remove(rw_array_t *a, unsigned index)
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
                    free(v.strv.str);
                    v.strv.str = 0;
                    break;
                case T_ARR:
                    destroy_rw_array(v.arrayv);
                    v.arrayv = 0;
                    break;
                case T_DICT:
                    destroy_rw_dict(v.dictv);
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

void dict_remove(rw_dict_t *d, string_t key)
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
            rw_item_t it = link->items[i];
            if (it.type != T_ERROR && strings_equals(key, it.key))
            {
                switch (it.type)
                {
                case T_STR:
                    free(it.strv.str);
                    it.strv.str = 0;
                    break;
                case T_ARR:
                    destroy_rw_array(it.arrayv);
                    it.arrayv = 0;
                    break;
                case T_DICT:
                    destroy_rw_dict(it.dictv);
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
rw_value_t array_get(rw_array_t *a, unsigned index)
{
    if (!a || index >= a->size)
    {
        return ERROR_RW_VALUE;
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
    return ERROR_RW_VALUE;
}

rw_item_t dict_get(rw_dict_t *d, string_t key)
{
    if (!d)
    {
        return ERROR_RW_ITEM;
    }

    item_link_t *link = d->head;
    unsigned non_null_values = 0;
    while (link)
    {
        for (unsigned i = 0; i < ARRAY_LEN; ++i)
        {
            rw_item_t it = link->items[i];
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
    return ERROR_RW_ITEM;
}

/*******************************************************************************
**                                   PRINTING                                 **
*******************************************************************************/
void rw_dict_print_indent(rw_dict_t *d, unsigned indent, char fromDict);
void rw_dict_print(rw_dict_t *d);

void rw_array_print_indent(rw_array_t *a, unsigned indent, char fromDict)
{
    if (!a)
    {
        return;
    }

    // Obtains the number of tab characters that will be printed
    char *tabs = calloc(indent, sizeof(char));
    if (!tabs)
    {
        return;
    }
    for (unsigned i = 0; i < indent - 1; ++i)
    {
        tabs[i] = '\t';
    }
    tabs[indent - 1] = '\0';

    unsigned size = a->size;
    // Empty array
    if (size == 0)
    {
        printf("%s[]", fromDict ? "" : tabs);
        free(tabs);
        return;
    }

    printf("%s[\n", fromDict ? "" : tabs);

    unsigned b = 0;
    value_link_t *link = a->head;
    while (link)
    {
        rw_value_t *values = link->values;
        for (unsigned i = 0; i < ARRAY_LEN; ++i)
        {
            rw_value_t v = values[i];
            if (v.type == T_ERROR)
            {
                continue;
            }

            switch (v.type)
            {
            case T_STR:
                printf("\t%s\"%s\"", tabs, v.strv.str ? v.strv.str : "");
                break;
            case T_INT:
                printf("\t%s%d", tabs, v.intv);
                break;
            case T_DOUBLE:
                printf("\t%s%f", tabs, v.doublev);
                break;
            case T_BOOL:
                printf("\t%s%s", tabs, v.boolv ? "true" : "false");
                break;
            case T_NULL:
                printf("\t%snull", tabs);
                break;
            case T_ARR:
                rw_array_print_indent(v.arrayv, indent + 1, 0);
                break;
            case T_DICT:
                rw_dict_print_indent(v.dictv, indent + 1, 0);
                break;
            }

            if (b++ < size - 1)
            {
                printf(",\n");
            }
        }
        link = link->next;
    }
    printf("\n%s]", tabs);
    free(tabs);
}

void rw_array_print(rw_array_t *a)
{
#ifndef VALGRING_DISABLE_PRINT
    if (a)
    {
        rw_array_print_indent(a, 1, 0);
    }
#endif
}

void rw_dict_print_indent(rw_dict_t *d, unsigned indent, char fromDict)
{
    // Obtains the number of tab characters that will be printed
    char *tabs = calloc(indent, sizeof(char));
    if (!tabs)
    {
        return;
    }
    for (unsigned i = 0; i < indent - 1; ++i)
    {
        tabs[i] = '\t';
    }
    tabs[indent - 1] = '\0';

    unsigned size = d->size;
    if (size == 0)
    {
        printf("%s{}", fromDict ? "" : tabs);
        free(tabs);
        return;
    }

    printf("%s{\n", fromDict ? "" : tabs);

    unsigned i = 0;
    item_link_t *link = d->head;
    while (link)
    {
        rw_item_t *items = link->items;
        for (unsigned a = 0; a < ARRAY_LEN; ++a, ++i)
        {
            rw_item_t it = items[a];
            string_t key = it.key;
            if (it.type == T_ERROR)
            {
                continue;
            }

            switch (it.type)
            {
            case T_STR:
                printf("\t%s\"%s\" : \"%s\"", tabs, key.str,
                       it.strv.str ? it.strv.str : "");
                break;
            case T_INT:
                printf("\t%s\"%s\" : %d", tabs, key.str, it.intv);
                break;
            case T_DOUBLE:
                printf("\t%s\"%s\" : %f", tabs, key.str, it.doublev);
                break;
            case T_BOOL:
                printf("\t%s\"%s\" : %s", tabs, key.str,
                       it.boolv ? "true" : "false");
                break;
            case T_NULL:
                printf("\t%s\"%s\" : null", tabs, key.str);
                break;
            case T_ARR:
                printf("\t%s\"%s\" : ", tabs, key.str);
                rw_array_print_indent(it.arrayv, indent + 1, 1);
                break;
            case T_DICT:
                printf("\t%s\"%s\" : ", tabs, key.str);
                rw_dict_print_indent(it.dictv, indent + 1, 1);
                break;
            }

            if (i < size - 1)
            {
                printf(",\n");
            }
        }
        link = link->next;
    }
    printf("\n%s}", tabs);
    free(tabs);
}

void rw_dict_print(rw_dict_t *d)
{
#ifndef VALGRING_DISABLE_PRINT
    if (d)
    {
        rw_dict_print_indent(d, 1, 0);
    }
#endif
}

/*******************************************************************************
**                                 DESTRUCTION                                **
*******************************************************************************/
void destroy_rw_array(rw_array_t *a)
{
    if (!a)
    {
        return;
    }

    value_link_t *link = a->head;
    while (link)
    {
        value_link_t *tmp = link;
        rw_value_t *values = link->values;
        link = link->next;
        for (unsigned i = 0; i < ARRAY_LEN; ++i)
        {
            switch (values[i].type)
            {
            case T_STR:
                free(values[i].strv.str);
                break;
            case T_ARR:
                destroy_rw_array(values[i].arrayv);
                break;
            case T_DICT:
                destroy_rw_dict(values[i].dictv);
                break;
            }
        }
        free(tmp);
    }
    free(a);
}

void destroy_rw_dict(rw_dict_t *d)
{
    if (!d)
    {
        return;
    }

    item_link_t *link = d->head;
    while (link)
    {
        item_link_t *tmp = link;
        rw_item_t *items = link->items;
        link = link->next;
        for (unsigned i = 0; i < ARRAY_LEN; ++i)
        {
            switch (items[i].type)
            {
            case T_STR:
                free(items[i].strv.str);
                break;
            case T_ARR:
                destroy_rw_array(items[i].arrayv);
                break;
            case T_DICT:
                destroy_rw_dict(items[i].dictv);
                break;
            }
            free(items[i].key.str);
        }
        free(tmp);
    }
    free(d);
}

void destroy_rw_json(rw_json_t *j)
{
    if (!j)
    {
        return;
    }

    if (j->is_array)
    {
        destroy_rw_array(j->array);
    }
    else
    {
        destroy_rw_dict(j->dict);
    }

    free(j);
}
