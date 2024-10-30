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

#define ARRAY_ADD(T_TYPE)                                                      \
    ValueLink *vl = calloc(1, sizeof(ValueLink));                              \
    vl->type = T_TYPE;                                                         \
    if (!a->head)                                                              \
    {                                                                          \
        a->head = vl;                                                          \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        a->tail->next = vl;                                                    \
    }                                                                          \
    a->tail = vl;                                                              \
    ++a->size

#define DICT_ADD(T_TYPE)                                                       \
    ItemLink *il = calloc(1, sizeof(ItemLink));                                \
    il->type = T_TYPE;                                                         \
    il->key = key;                                                             \
    if (!d->head)                                                              \
    {                                                                          \
        d->head = il;                                                          \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        d->tail->next = il;                                                    \
    }                                                                          \
    d->tail = il;                                                              \
    ++d->size

#define ARRAY_INSERT(T_TYPE)                                                   \
    /* If index is equal to the size, it is the same as adding an element      \
     * or if the size is 0 but the head is not null (which may have been       \
     * caused by an error)*/                                                   \
    if (index > a->size || (a->size == 0 && a->head))                          \
    {                                                                          \
        return;                                                                \
    }                                                                          \
    ValueLink *vl = calloc(1, sizeof(ValueLink));                              \
    vl->type = T_TYPE;                                                         \
    /* If the linked list is empty */                                          \
    if (!a->head)                                                              \
    {                                                                          \
        a->head = vl;                                                          \
        a->tail = vl;                                                          \
    }                                                                          \
    else if (index == 0)                                                       \
    {                                                                          \
        vl->next = a->head;                                                    \
        a->head = vl;                                                          \
    }                                                                          \
    else if (index == a->size)                                                 \
    {                                                                          \
        a->tail->next = vl;                                                    \
        a->tail = vl;                                                          \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        /* Navigates the the element at position 'index - 1', make its next    \
        element the next of the new element, and make its next point to        \
        the new element */                                                     \
        ValueLink *link = a->head;                                             \
        while (link && --index)                                                \
        {                                                                      \
            link = link->next;                                                 \
        }                                                                      \
        vl->next = link->next;                                                 \
        link->next = vl;                                                       \
    }                                                                          \
    ++a->size

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
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
void arr_add_str(Array *a, String value)
{
    if (a)
    {
        ARRAY_ADD(T_STR);
        vl->strv = value;
    }
}

void arr_add_int(Array *a, int value)
{
    if (a)
    {
        ARRAY_ADD(T_INT);
        vl->intv = value;
    }
}

void arr_add_double(Array *a, double value)
{
    if (a)
    {
        ARRAY_ADD(T_DOUBLE);
        vl->doublev = value;
    }
}

void arr_add_bool(Array *a, char value)
{
    if (a)
    {
        ARRAY_ADD(T_BOOL);
        vl->boolv = value;
    }
}

void arr_add_null(Array *a)
{
    if (a)
    {
        ARRAY_ADD(T_NULL);
    }
}

void arr_add_arr(Array *a, Array *value)
{
    if (a && value)
    {
        ARRAY_ADD(T_ARR);
        vl->arrayv = value;
    }
}

void arr_add_dict(Array *a, Dict *value)
{
    if (a && value)
    {
        ARRAY_ADD(T_DICT);
        vl->dictv = value;
    }
}

void dict_add_str(Dict *d, String key, String value)
{
    if (d)
    {
        DICT_ADD(T_STR);
        il->strv = value;
    }
}

void dict_add_int(Dict *d, String key, int value)
{
    if (d)
    {
        DICT_ADD(T_INT);
        il->intv = value;
    }
}

void dict_add_double(Dict *d, String key, double value)
{
    if (d)
    {
        DICT_ADD(T_DOUBLE);
        il->doublev = value;
    }
}

void dict_add_bool(Dict *d, String key, char value)
{
    if (d)
    {
        DICT_ADD(T_BOOL);
        il->boolv = value;
    }
}

void dict_add_null(Dict *d, String key)
{
    if (d)
    {
        DICT_ADD(T_NULL);
    }
}

void dict_add_arr(Dict *d, String key, Array *value)
{
    if (d && value)
    {
        DICT_ADD(T_ARR);
        il->arrayv = value;
    }
}

void dict_add_dict(Dict *d, String key, Dict *value)
{
    if (d && value)
    {
        DICT_ADD(T_DICT);
        il->dictv = value;
    }
}

/*******************************************************************************
**                                   REMOVES                                  **
*******************************************************************************/
void arr_remove(Array *a, unsigned index)
{
    if (a && index < a->size && a->head)
    {
        ValueLink *link = a->head;
        while (link && --index)
        {
            link = link->next;
        }

        // If index is not 0, it means that we encountered a null link, and it
        // is an error
        if (index)
        {
            return;
        }

        // Takes the link that is before the one we want to remove and make its
        // next point to the next of the link we want to remove
        if (link->next)
        {
            ValueLink *tmp = link->next;
            link->next = link->next->next;

            switch (tmp->type)
            {
            case T_STR:
                free(tmp->strv.str);
                break;
            case T_ARR:
                destroy_array(tmp->arrayv);
                break;
            case T_DICT:
                destroy_dict(tmp->dictv);
                break;
            }
            free(tmp);
            --a->size;
        }
    }
}

void dict_remove(Dict *d, String key)
{
    if (d && d->head)
    {
        ItemLink *link = d->head;
        while (link)
        {
            if (strings_equals(key, link->key))
            {
                // Takes the link that is before the one we want to remove and
                // make its next point to the next of the link we want to remove
                if (link->next)
                {
                    ItemLink *tmp = link->next;
                    link->next = link->next->next;

                    switch (tmp->type)
                    {
                    case T_STR:
                        free(tmp->strv.str);
                        break;
                    case T_ARR:
                        destroy_array(tmp->arrayv);
                        break;
                    case T_DICT:
                        destroy_dict(tmp->dictv);
                        break;
                    }
                    free(tmp);
                }
                --d->size;
                break;
            }
            link = link->next;
        }
    }
}

/*******************************************************************************
**                                   INSERTS                                  **
*******************************************************************************/
void arr_insert_str(Array *a, unsigned index, String value)
{
    if (a)
    {
        ARRAY_INSERT(T_STR);
        vl->strv = value;
    }
}

void arr_insert_int(Array *a, unsigned index, int value)
{
    if (a)
    {
        ARRAY_INSERT(T_INT);
        vl->intv = value;
    }
}

void arr_insert_double(Array *a, unsigned index, double value)
{
    if (a)
    {
        ARRAY_INSERT(T_DOUBLE);
        vl->doublev = value;
    }
}

void arr_insert_bool(Array *a, unsigned index, char value)
{
    if (a)
    {
        ARRAY_INSERT(T_BOOL);
        vl->boolv = value;
    }
}

void arr_insert_null(Array *a, unsigned index)
{
    if (a)
    {
        ARRAY_INSERT(T_NULL);
    }
}

void arr_insert_arr(Array *a, unsigned index, Array *value)
{
    if (a && value)
    {
        ARRAY_INSERT(T_ARR);
        vl->arrayv = value;
    }
}

void arr_insert_dict(Array *a, unsigned index, Dict *value)
{
    if (a && value)
    {
        ARRAY_INSERT(T_DICT);
        vl->dictv = value;
    }
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

    ValueLink *link = a->head;
    while (link && index--)
    {
        link = link->next;
    }

    // If index is 0, it means that we encountered a null link, and it
    // is an error
    if (!index)
    {
        return ERROR_VALUE;
    }

    if (link)
    {
        switch (link->type)
        {
        case T_STR:
            return (Value){ .type = T_STR, .strv = link->strv };
        case T_INT:
            return (Value){ .type = T_INT, .intv = link->intv };
        case T_DOUBLE:
            return (Value){ .type = T_DOUBLE, .doublev = link->doublev };
        case T_BOOL:
            return (Value){ .type = T_BOOL, .boolv = link->boolv };
        case T_NULL:
            return (Value){ .type = T_NULL };
        case T_ARR:
            return (Value){ .type = T_ARR, .arrayv = link->arrayv };
        case T_DICT:
            return (Value){ .type = T_DICT, .dictv = link->dictv };
        }
    }
    return ERROR_VALUE;
}

Item dict_get(Dict *d, String key)
{
    if (!d)
    {
        return ERROR_ITEM;
    }

    ItemLink *link = d->head;
    while (link)
    {
        if (strings_equals(key, link->key))
        {
            switch (link->type)
            {
            case T_STR:
                return (Item){ .type = T_STR, .key = key, .strv = link->strv };
            case T_INT:
                return (Item){ .type = T_INT, .key = key, .intv = link->intv };
            case T_DOUBLE:
                return (Item){ .type = T_DOUBLE,
                               .key = key,
                               .doublev = link->doublev };
            case T_BOOL:
                return (
                    Item){ .type = T_BOOL, .key = key, .boolv = link->boolv };
            case T_NULL:
                return (Item){ .type = T_NULL, .key = key };
            case T_ARR:
                return (
                    Item){ .type = T_ARR, .key = key, .arrayv = link->arrayv };
            case T_DICT:
                return (
                    Item){ .type = T_DICT, .key = key, .dictv = link->dictv };
            }
            break;
        }
        link = link->next;
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

    ValueLink *link = a->head;
    while (link)
    {
        ValueLink *tmp = link;
        link = link->next;

        switch (tmp->type)
        {
        case T_STR:
            free(tmp->strv.str);
            break;
        case T_ARR:
            destroy_array(tmp->arrayv);
            break;
        case T_DICT:
            destroy_dict(tmp->dictv);
            break;
        }
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

    ItemLink *link = d->head;
    while (link)
    {
        ItemLink *tmp = link;
        link = link->next;

        switch (tmp->type)
        {
        case T_STR:
            free(tmp->strv.str);
            break;
        case T_ARR:
            destroy_array(tmp->arrayv);
            break;
        case T_DICT:
            destroy_dict(tmp->dictv);
            break;
        }
        free(tmp->key.str);
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
