#include "linked_lists.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
/**
** \param link_type The typedef struct
** \param st The name of the variable of the type 'st_name'
** \param elt The element to add
** \param list the name of the list in which to add 'elt'
** \param nb_type The name of the field containing the number of elements
*/
#define APPEND(link_type, st, elt, list, nb_type)                              \
    if (st == NULL)                                                            \
    {                                                                          \
        return NULL;                                                           \
    }                                                                          \
    /* Case where there is no element */                                       \
    if (st->head == NULL)                                                      \
    {                                                                          \
        st->head = calloc(1, sizeof(struct link_type));                        \
        if (st->head == NULL)                                                  \
        {                                                                      \
            return NULL;                                                       \
        }                                                                      \
        st->head->list[st->idx] = elt;                                         \
    }                                                                          \
    /* Case where the current array is full */                                 \
    else if (st->idx == ARRAY_LEN)                                             \
    {                                                                          \
        /* We allocate the new element, add it at the end of the linked list   \
           of its type and adds to it the given key */                         \
        struct link_type *nkl = calloc(1, sizeof(struct link_type));           \
        if (nkl == NULL)                                                       \
        {                                                                      \
            return NULL;                                                       \
        }                                                                      \
        struct link_type *tmp = st->head;                                      \
        while (tmp->next != NULL)                                              \
        {                                                                      \
            tmp = tmp->next;                                                   \
        }                                                                      \
        tmp->next = nkl;                                                       \
        st->idx = 0;                                                           \
        nkl->list[st->idx] = elt;                                              \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        /* Adds the given key to the last array */                             \
        struct link_type *tmp = st->head;                                      \
        while (tmp->next != NULL)                                              \
        {                                                                      \
            tmp = tmp->next;                                                   \
        }                                                                      \
        tmp->list[st->idx] = elt;                                              \
    }                                                                          \
    ++st->nb_type;                                                             \
    struct link_type *tmp = st->head;                                          \
    while (tmp->next != NULL)                                                  \
    {                                                                          \
        tmp = tmp->next;                                                       \
    }

#define DESTROY(link_type, st)                                                 \
    if (st == NULL)                                                            \
    {                                                                          \
        return;                                                                \
    }                                                                          \
    struct link_type *tmp = st->head;                                          \
    while (tmp != NULL)                                                        \
    {                                                                          \
        struct link_type *to_del = tmp;                                        \
        tmp = tmp->next;                                                       \
        free(to_del);                                                          \
    }                                                                          \
    free(st);

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
/***************************************
**               PAIRS                **
***************************************/
struct pair *append_pair(pair_control_st *pc, struct pair *p)
{
    if (p == NULL)
    {
        return NULL;
    }
    APPEND(pair_array_link, pc, p, pairs, nb_pairs)
    return tmp->pairs[pc->idx++];
}

void print_pairs(pair_control_st *pc)
{
    if (pc == NULL || pc->head == NULL || pc->nb_pairs == 0)
    {
        return;
    }

    struct pair_array_link *array = pc->head;
    printf("{\n");
    // Iterates over the arrays
    while (array != NULL)
    {
        // Iterates over an array
        for (size_t i = 0; i < (array->next == NULL ? pc->idx : ARRAY_LEN); ++i)
        {
            if (array->pairs[i] != NULL)
            {
                long num = 0;
                switch (array->pairs[i]->type)
                {
                case TYPE_STR:
                    printf("\t\"%s\": \"%s\"", array->pairs[i]->key,
                           (const char *)array->pairs[i]->value);
                    break;
                case TYPE_NUM:
                    num = *(long *)array->pairs[i]->value;
                    printf("\t\"%s\": %ld", array->pairs[i]->key, num);
                    break;
                default:
                    break;
                }
                // If we are not at the last element of the last array, we print
                // a ','
                if ((array->next == NULL && i != pc->idx - 1)
                    || array->next != NULL)
                {
                    printf(",\n");
                }
            }
        }
        array = array->next;
    }
    printf("\n}\n");
}

void destroy_pair_control(pair_control_st *pc)
{
    if (pc == NULL)
    {
        return;
    }

    struct pair_array_link *tmp = pc->head;
    while (tmp != NULL)
    {
        struct pair_array_link *to_del = tmp;
        tmp = tmp->next;
        for (size_t i = 0; i < ARRAY_LEN; ++i)
        {
            free(to_del->pairs[i]);
        }
        free(to_del);
    }
    free(pc);
}

/***************************************
**                KEY                 **
***************************************/
const char *append_key(key_control_st *kc, const char *key)
{
    if (key == NULL)
    {
        return NULL;
    }
    APPEND(key_array_link, kc, key, keys, nb_keys)
    return tmp->keys[kc->idx++];
}

void destroy_key_control(key_control_st *kc)
{
    DESTROY(key_array_link, kc)
}

/***************************************
**                STR                 **
***************************************/
const char *append_str(str_control_st *sc, const char *str)
{
    if (str == NULL)
    {
        return NULL;
    }
    APPEND(str_array_link, sc, str, strings, nb_str)
    return tmp->strings[sc->idx++];
}

void destroy_str_control(str_control_st *sc)
{
    DESTROY(str_array_link, sc)
}

/***************************************
**                NUM                 **
***************************************/
const long *append_num(num_control_st *nc, long num)
{
    APPEND(num_array_link, nc, num, numbers, nb_num)
    return &tmp->numbers[nc->idx++];
}

void destroy_num_control(num_control_st *nc)
{
    DESTROY(num_array_link, nc)
}

/***************************************
**               BOOL                 **
***************************************/
const char *append_bool(bool_control_st *bc, char boolean)
{
    APPEND(bool_array_link, bc, boolean, booleans, nb_bool)
    return &tmp->booleans[bc->idx++];
}

void destroy_bool_control(bool_control_st *bc)
{
    DESTROY(bool_array_link, bc)
}
