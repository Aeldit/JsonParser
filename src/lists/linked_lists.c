#include "linked_lists.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../types.h"
#include "json_array.h"

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
/**
** \brief Destroys the given linked list
** \param ctrl_type The typedef struct for the ctrl variable
** \param link_type The typedef struct
** \param list the name of the list in which to add 'elt'
** \param nb_type The name of the field containing the number of elements
** \param is_key If we are inserting a key, we don't need to increment the
**               number of pairs, as it is done when adding the associated pair
*/
#define APPEND(ctrl_type, link_type, list, nb_type, is_key)                    \
    ctrl_type *ctrl = jd->list;                                                \
    if (ctrl == NULL)                                                          \
    {                                                                          \
        printf("a");                                                           \
        return NULL;                                                           \
    }                                                                          \
    /* Case where there is no element */                                       \
    if (ctrl->head == NULL)                                                    \
    {                                                                          \
        ctrl->head = calloc(1, sizeof(struct link_type));                      \
        if (ctrl->head == NULL)                                                \
        {                                                                      \
            printf("b");                                                       \
            return NULL;                                                       \
        }                                                                      \
        ctrl->head->list[ctrl->idx] = value;                                   \
    }                                                                          \
    /* Case where the current array is full */                                 \
    else if (ctrl->idx == ARRAY_LEN)                                           \
    {                                                                          \
        /* We allocate the new element, add it at the end of the linked list   \
           of its type and adds to it the given key */                         \
        struct link_type *nkl = calloc(1, sizeof(struct link_type));           \
        if (nkl == NULL)                                                       \
        {                                                                      \
            printf("c");                                                       \
            return NULL;                                                       \
        }                                                                      \
        struct link_type *tmp = ctrl->head;                                    \
        while (tmp->next != NULL)                                              \
        {                                                                      \
            tmp = tmp->next;                                                   \
        }                                                                      \
        tmp->next = nkl;                                                       \
        ctrl->idx = 0;                                                         \
        nkl->list[ctrl->idx] = value;                                          \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        /* Adds the given key to the last array */                             \
        struct link_type *tmp = ctrl->head;                                    \
        while (tmp->next != NULL)                                              \
        {                                                                      \
            tmp = tmp->next;                                                   \
        }                                                                      \
        tmp->list[ctrl->idx] = value;                                          \
    }                                                                          \
    if (is_key)                                                                \
    {                                                                          \
        ++nb_type;                                                             \
    }                                                                          \
    struct link_type *tmp = ctrl->head;                                        \
    while (tmp->next != NULL)                                                  \
    {                                                                          \
        tmp = tmp->next;                                                       \
    }

/*
** \brief Destroys the given linked list
** \param link_type The typedef struct
** \param list The name of the list field in the structure link_type
*/
#define DESTROY(type_link, list)                                               \
    if (ctrl == NULL)                                                          \
    {                                                                          \
        return;                                                                \
    }                                                                          \
    struct type_link *tmp = ctrl->head;                                        \
    while (tmp != NULL)                                                        \
    {                                                                          \
        struct type_link *to_del = tmp;                                        \
        tmp = tmp->next;                                                       \
        for (int i = 0; i < ARRAY_LEN; ++i)                                    \
        {                                                                      \
            free(to_del->list[i]);                                             \
        }                                                                      \
        free(to_del);                                                          \
    }                                                                          \
    free(ctrl);

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
/***********************************************************
**                       LILFECYCLE                       **
***********************************************************/
/***************************************
**               PAIR                 **
***************************************/
struct pair *append_pair(json_dict_st *jd, struct pair *value)
{
    if (jd == NULL || value == NULL)
    {
        return NULL;
    }
    APPEND(pair_control_st, pair_link, pairs, jd->nb_pairs, 0)
    return tmp->pairs[ctrl->idx++];
}

void destroy_pair_control(pair_control_st *ctrl)
{
    DESTROY(pair_link, pairs)
}

/***************************************
**                KEY                 **
***************************************/
char *append_key(json_dict_st *jd, char *value)
{
    if (jd == NULL || value == NULL)
    {
        return NULL;
    }
    APPEND(key_control_st, key_link, keys, jd->nb_pairs, 1)
    return tmp->keys[jd->keys->idx++];
}

void destroy_key_control(key_control_st *ctrl)
{
    DESTROY(key_link, keys)
}

/***************************************
**                STR                 **
***************************************/
char *append_str(json_dict_st *jd, char *value)
{
    if (jd == NULL || value == NULL)
    {
        return NULL;
    }
    APPEND(str_control_st, str_link, strings, jd->strings->nb_str, 0)
    return tmp->strings[ctrl->idx++];
}

void destroy_str_control(str_control_st *ctrl)
{
    DESTROY(str_link, strings)
}

/***************************************
**                NUM                 **
***************************************/
long *append_num(json_dict_st *jd, long value)
{
    if (jd == NULL)
    {
        return NULL;
    }
    APPEND(num_control_st, num_link, numbers, jd->numbers->nb_num, 0)
    return &tmp->numbers[ctrl->idx++];
}

void destroy_num_control(num_control_st *ctrl)
{
    if (ctrl == NULL)
    {
        return;
    }
    struct num_link *tmp = ctrl->head;
    while (tmp != NULL)
    {
        struct num_link *to_del = tmp;
        tmp = tmp->next;
        free(to_del);
    }
    free(ctrl);
}

/***************************************
**             JSON DICT              **
***************************************/
json_dict_st *append_json_dict(json_dict_st *jd, json_dict_st *value)
{
    if (jd == NULL || value == NULL)
    {
        return NULL;
    }
    APPEND(json_dict_control_st, json_dict_link, json_dicts,
           jd->json_dicts->nb_json_dicts, 0)
    return tmp->json_dicts[ctrl->idx++];
}

void destroy_json_dict_control(json_dict_control_st *ctrl)
{
    if (ctrl == NULL)
    {
        return;
    }

    struct json_dict_link *tmp = ctrl->head;
    while (tmp != NULL)
    {
        struct json_dict_link *to_del = tmp;
        tmp = tmp->next;
        for (int i = 0; i < ARRAY_LEN; ++i)
        {
            destroy_dict(to_del->json_dicts[i]);
        }
        free(to_del);
    }
    free(ctrl);
}

/***************************************
**               ARRAY                **
***************************************/
json_array_st *append_array(json_dict_st *jd, json_array_st *value)
{
    if (jd == NULL || value == NULL)
    {
        return NULL;
    }
    APPEND(list_control_st, array_link, lists, jd->lists->nb_arrays, 0)
    return tmp->lists[ctrl->idx++];
}

void destroy_array_control(list_control_st *ctrl)
{
    if (ctrl == NULL)
    {
        return;
    }

    struct array_link *tmp = ctrl->head;
    while (tmp != NULL)
    {
        struct array_link *to_del = tmp;
        tmp = tmp->next;
        for (int i = 0; i < ARRAY_LEN; ++i)
        {
            if (to_del->lists[i] != NULL)
            {
                free(to_del->lists[i]->elts);
            }
            free(to_del->lists[i]);
        }
        free(to_del);
    }
    free(ctrl);
}

/***************************************
**               BOOL                 **
***************************************/
char *append_bool(json_dict_st *jd, char value)
{
    if (jd == NULL)
    {
        return NULL;
    }
    APPEND(bool_control_st, bool_link, booleans, jd->booleans->nb_bool, 0)
    return &tmp->booleans[ctrl->idx++];
}

void destroy_bool_control(bool_control_st *ctrl)
{
    if (ctrl == NULL)
    {
        return;
    }
    struct bool_link *tmp = ctrl->head;
    while (tmp != NULL)
    {
        struct bool_link *to_del = tmp;
        tmp = tmp->next;
        free(to_del);
    }
    free(ctrl);
}

/***********************************************************
**                         UTILS                          **
***********************************************************/
char key_exists(json_dict_st *jd, char *key)
{
    if (jd == NULL || key == NULL || jd->keys == NULL)
    {
        return 0;
    }

    for (size_t i = 0; i < jd->nb_pairs; ++i)
    {
        struct key_link *tmp = jd->keys->head;
        while (tmp != NULL)
        {
            for (size_t j = 0;
                 j < (tmp->next == NULL ? jd->keys->idx : ARRAY_LEN); ++j)
            {
                if (strcmp(tmp->keys[j], key) == 0)
                {
                    return 1;
                }
            }
            tmp = tmp->next;
        }
    }
    return 0;
}

typed_value_st get_value(json_dict_st *jd, char *key)
{
    if (jd == NULL || key == NULL)
    {
        return (typed_value_st){ .type = TYPE_ERROR, .value = NULL };
    }

    // Iterates over the pairs
    struct pair_link *tmp = jd->pairs->head;
    while (tmp != NULL)
    {
        // Iterates over the keys
        for (size_t j = 0; j < (tmp->next == NULL ? jd->pairs->idx : ARRAY_LEN);
             ++j)
        {
            if (strcmp(tmp->pairs[j]->key, key) == 0)
            {
                return (typed_value_st){ .type = tmp->pairs[j]->type,
                                         .value = tmp->pairs[j]->value };
            }
        }
        tmp = tmp->next;
    }
    return (typed_value_st){ .type = TYPE_ERROR, .value = NULL };
}

/***********************************************************
**                    LINKED LISTS STR                    **
***********************************************************/
ll_char_ctrl_st *init_ll(void)
{
    ll_char_ctrl_st *llcc = calloc(1, sizeof(ll_char_ctrl_st));
    if (llcc == NULL)
    {
        return NULL;
    }
    return llcc;
}

void add_char_to_ll(ll_char_ctrl_st *llcc, char c)
{
    if (llcc == NULL)
    {
        return;
    }

    if (llcc->head == NULL)
    {
        llcc->head = calloc(1, sizeof(struct linked_list_char));
        if (llcc->head == NULL)
        {
            return;
        }
        llcc->idx = 0;
        llcc->head->buff[llcc->idx++] = c;
    }
    else if (llcc->idx == CHAR_BUFF_LEN)
    {
        struct linked_list_char *llc =
            calloc(1, sizeof(struct linked_list_char));
        if (llc == NULL)
        {
            return;
        }

        struct linked_list_char *tmp = llcc->head;
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        tmp->next = llc;
        llcc->idx = 0;
        llc->buff[llcc->idx++] = c;
    }
    else
    {
        struct linked_list_char *tmp = llcc->head;
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        tmp->buff[llcc->idx++] = c;
    }
    ++llcc->len;
}

char *get_final_string(ll_char_ctrl_st *llcc)
{
    if (llcc == NULL)
    {
        return NULL;
    }

    // If the length is 0, we simply ignore the current string and errase it
    if (llcc->len == 0)
    {
        struct linked_list_char *tmp = llcc->head;
        while (tmp != NULL)
        {
            struct linked_list_char *to_del = tmp;
            tmp = tmp->next;
            free(to_del);
        }
        llcc->len = 0;
        llcc->idx = 0;
        llcc->head = NULL;
        return NULL;
    }

    // + 1 for the '\0'
    char *str = calloc(llcc->len + 1, sizeof(char));
    if (str == NULL)
    {
        return NULL;
    }
    size_t idx = 0;

    struct linked_list_char *tmp = llcc->head;
    while (tmp != NULL)
    {
        struct linked_list_char *to_del = tmp;
        for (int i = 0; i < CHAR_BUFF_LEN; ++i)
        {
            str[idx + i] = tmp->buff[i];
            tmp->buff[i] = 0;
        }
        idx += CHAR_BUFF_LEN;
        tmp = tmp->next;
        free(to_del);
    }
    str[llcc->len] = '\0';
    llcc->len = 0;
    llcc->idx = 0;
    llcc->head = NULL;
    return str;
}

void destroy_llcc(ll_char_ctrl_st *llcc)
{
    if (llcc == NULL)
    {
        return;
    }

    struct linked_list_char *tmp = llcc->head;
    while (tmp != NULL)
    {
        struct linked_list_char *to_del = tmp;
        tmp = tmp->next;
        free(to_del);
    }
    free(llcc);
}
