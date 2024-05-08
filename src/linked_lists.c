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
** \param err_ret The value to return on error
*/
#define APPEND(link_type, st, elt, list, nb_type, err_ret)                     \
    if (st == NULL)                                                            \
    {                                                                          \
        return err_ret;                                                        \
    }                                                                          \
    /* Case where there is no element */                                       \
    if (st->head == NULL)                                                      \
    {                                                                          \
        st->head = calloc(1, sizeof(struct link_type));                        \
        if (st->head == NULL)                                                  \
        {                                                                      \
            return err_ret;                                                    \
        }                                                                      \
        st->head->list[st->idx++] = elt;                                       \
    }                                                                          \
    /* Case where the current array is full */                                 \
    else if (st->idx == ARRAY_LEN)                                             \
    {                                                                          \
        /* We allocate the new element, add it at the end of the linked list   \
           of its type and adds to it the given key */                         \
        struct link_type *nkl = calloc(1, sizeof(struct link_type));           \
        if (nkl == NULL)                                                       \
        {                                                                      \
            return err_ret;                                                    \
        }                                                                      \
        struct link_type *tmp = st->head;                                      \
        while (tmp->next != NULL)                                              \
        {                                                                      \
            tmp = tmp->next;                                                   \
        }                                                                      \
        tmp->next = nkl;                                                       \
        st->idx = 0;                                                           \
        nkl->list[st->idx++] = elt;                                            \
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
    APPEND(pair_array_link, pc, p, pairs, nb_pairs, NULL)
    return tmp->pairs[pc->idx++];
}

void print_pairs(pair_control_st *pc)
{
    if (pc == NULL || pc->head == NULL || pc->nb_pairs == 0)
    {
        return;
    }

    struct pair_array_link *tmp = pc->head;
    printf("{\n");
    // Iterates over the arrays
    while (tmp != NULL)
    {
        // Iterates over an array
        for (size_t i = 0; i < (tmp->next == NULL ? pc->idx : ARRAY_LEN); ++i)
        {
            switch (tmp->pairs[i]->type)
            {
            case TYPE_STR:
                printf("\t\"%s\": \"%s\"", tmp->pairs[i]->key,
                       (const char *)tmp->pairs[i]->value);
                break;
            case TYPE_NUM:
                printf("\t\"%s\": \"%ld\"", tmp->pairs[i]->key,
                       (long)tmp->pairs[i]->value);
                break;
            default:
                break;
            }
            // If we are not at the last element of the last array, we print a
            // ','
            if ((tmp->next == NULL && i != pc->idx - 1) || tmp->next != NULL)
            {
                printf(",\n");
            }
        }
        tmp = tmp->next;
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
    APPEND(key_array_link, kc, key, keys, nb_keys, NULL)
    return tmp->keys[kc->idx++];
}

void print_keys(key_control_st *kc)
{
    if (kc == NULL || kc->head == NULL || kc->nb_keys == 0)
    {
        return;
    }

    struct key_array_link *tmp = kc->head;
    printf("[ ");
    // Iterates over the arrays
    while (tmp != NULL)
    {
        // Iterates over an array
        for (size_t i = 0; i < (tmp->next == NULL ? kc->idx : ARRAY_LEN); ++i)
        {
            printf("\"%s\"", tmp->keys[i]);
            // If we are not at the last element of the last array, we print a
            // ','
            if ((tmp->next == NULL && i != kc->idx - 1) || tmp->next != NULL)
            {
                printf(", ");
            }
        }
        tmp = tmp->next;
    }
    printf(" ]\n");
}

void destroy_key_control(key_control_st *kc)
{
    if (kc == NULL)
    {
        return;
    }

    struct key_array_link *tmp = kc->head;
    while (tmp != NULL)
    {
        struct key_array_link *to_del = tmp;
        tmp = tmp->next;
        free(to_del);
    }
    free(kc);
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
    APPEND(str_array_link, sc, str, strings, nb_str, NULL)
    return tmp->strings[sc->idx++];
}

void destroy_str_control(str_control_st *sc)
{
    if (sc == NULL)
    {
        return;
    }

    struct str_array_link *tmp = sc->head;
    while (tmp != NULL)
    {
        struct str_array_link *to_del = tmp;
        tmp = tmp->next;
        free(to_del);
    }
    free(sc);
}

/***************************************
**                NUM                 **
***************************************/
long *append_num(num_control_st *nc, long num)
{
    APPEND(num_array_link, nc, num, numbers, nb_num, NULL)
    return &tmp->numbers[nc->idx++];
}

void destroy_num_control(num_control_st *nc)
{
    if (nc == NULL)
    {
        return;
    }

    struct num_array_link *tmp = nc->head;
    while (tmp != NULL)
    {
        struct num_array_link *to_del = tmp;
        tmp = tmp->next;
        free(to_del);
    }
    free(nc);
}
