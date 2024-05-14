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
        return NULL;                                                           \
    }                                                                          \
    /* Case where there is no element */                                       \
    if (ctrl->head == NULL)                                                    \
    {                                                                          \
        ctrl->head = calloc(1, sizeof(struct link_type));                      \
        if (ctrl->head == NULL)                                                \
        {                                                                      \
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
*/
#define DESTROY(link_type)                                                     \
    if (ctrl == NULL)                                                          \
    {                                                                          \
        return;                                                                \
    }                                                                          \
    struct link_type *tmp = ctrl->head;                                        \
    while (tmp != NULL)                                                        \
    {                                                                          \
        struct link_type *to_del = tmp;                                        \
        tmp = tmp->next;                                                       \
        free(to_del);                                                          \
    }                                                                          \
    free(ctrl);

/*******************************************************************************
**                              LOCAL FUNCTIONS                               **
*******************************************************************************/
void print_json_rec(pair_control_st *ctrl, char indent)
{
    if (ctrl == NULL || ctrl->head == NULL)
    {
        return;
    }

    char *tabs = calloc(indent + 1, sizeof(char));
    if (tabs == NULL)
    {
        return;
    }
    for (int i = 0; i < indent - 1; ++i)
    {
        tabs[i] = '\t';
    }

    struct pair_link *array = ctrl->head;
    printf("{\n");
    // Iterates over the arrays
    while (array != NULL)
    {
        // Iterates over an array
        for (size_t i = 0; i < (array->next == NULL ? ctrl->idx : ARRAY_LEN);
             ++i)
        {
            if (array->pairs[i] != NULL)
            {
                const char *key = array->pairs[i]->key;
                long num = 0;
                char boolean = 0;
                json_array_st *l = NULL;
                json_dict_st *j = NULL;
                switch (array->pairs[i]->type)
                {
                case TYPE_STR:
                    printf("%s\t\"%s\": \"%s\"", tabs, key,
                           (const char *)array->pairs[i]->value);
                    break;
                case TYPE_NUM:
                    num = *(long *)array->pairs[i]->value;
                    printf("%s\t\"%s\": %ld", tabs, key, num);
                    break;
                case TYPE_OBJ:
                    j = array->pairs[i]->value;
                    printf("%s\t\"%s\": ", tabs, key);
                    if (j == NULL)
                    {
                        printf("{}");
                    }
                    else
                    {
                        print_json_rec(j->pairs, indent + 1);
                    }
                    break;
                case TYPE_ARR:
                    l = array->pairs[i]->value;
                    printf("%s\t\"%s\": ", tabs, key);
                    array_print_indent(l, indent + 1, 0);
                    break;
                case TYPE_BOOL:
                    boolean = *(char *)array->pairs[i]->value;
                    printf("%s\t\"%s\": %s", tabs, key,
                           boolean ? "true" : "false");
                    break;
                case TYPE_NULL:
                    printf("%s\t\"%s\": null", tabs, key);
                    break;
                default:
                    break;
                }

                // If not at the last element of the last array, we print a ','
                if ((array->next == NULL && i != ctrl->idx - 1)
                    || array->next != NULL)
                {
                    printf(",\n");
                }
            }
        }
        array = array->next;
    }
    printf("\n%s}", tabs);
    if (indent == 1)
    {
        printf("\n");
    }
    free(tabs);
}

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

void print_json(pair_control_st *ctrl)
{
    print_json_rec(ctrl, 1);
}

void destroy_pair_control(pair_control_st *ctrl)
{
    if (ctrl == NULL)
    {
        return;
    }

    struct pair_link *tmp = ctrl->head;
    while (tmp != NULL)
    {
        struct pair_link *to_del = tmp;
        tmp = tmp->next;
        for (size_t i = 0; i < ARRAY_LEN; ++i)
        {
            free(to_del->pairs[i]);
        }
        free(to_del);
    }
    free(ctrl);
}

/***************************************
**                KEY                 **
***************************************/
const char *append_key(json_dict_st *jd, const char *value)
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
    DESTROY(key_link)
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
    DESTROY(str_link)
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

void destroy_num_control(num_control_st *ctrl){ DESTROY(num_link) }

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
        for (size_t i = 0; i < ARRAY_LEN; ++i)
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
        for (size_t i = 0; i < ARRAY_LEN; ++i)
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
    DESTROY(bool_link)
}

/***********************************************************
**                         UTILS                          **
***********************************************************/
char key_exists(json_dict_st *jd, const char *key)
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

typed_value_st get_value(json_dict_st *jd, const char *key)
{
    if (key == NULL)
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
                printf("%d\n", tmp->pairs[j]->type);
                return (typed_value_st){ .type = tmp->pairs[j]->type,
                                         .value = tmp->pairs[j]->value };
            }
        }
        tmp = tmp->next;
    }
    return (typed_value_st){ .type = TYPE_ERROR, .value = NULL };
}
